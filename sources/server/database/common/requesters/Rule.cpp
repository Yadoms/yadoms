#include "stdafx.h"
#include "Rule.h"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/DatabaseException.hpp"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CRule::CRule(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(std::move(databaseRequester))
         {
         }

         // IRuleRequester implementation

         std::vector<boost::shared_ptr<entities::CRule>> CRule::getRules() const
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRuleTable::getTableName());

            adapters::CRuleAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CRule>> CRule::getRules(
            const boost::optional<int>& ruleId,
            const boost::optional<std::string>& fromName,
            const std::set<std::string>& fromInterpreter,
            const boost::optional<std::string>& fromEditor,
            bool fromAutostart,
            const std::set<entities::ERuleState>& fromState) const
         {
            const auto query = m_databaseRequester->newQuery();

            query->Select().
                   From(CRuleTable::getTableName()).
                   WhereTrue();

            if (ruleId)
               query->And(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, *ruleId);
            if (fromName)
               query->And(CRuleTable::getNameColumnName(), CQUERY_OP_EQUAL, *fromName);
            if (!fromInterpreter.empty())
               query->And(CRuleTable::getInterpreterColumnName(), CQUERY_OP_IN, fromInterpreter);
            if (fromEditor)
               query->And(CRuleTable::getEditorColumnName(), CQUERY_OP_EQUAL, *fromEditor);
            if (fromAutostart)
               query->And(CRuleTable::getAutoStartColumnName(), CQUERY_OP_EQUAL, fromAutostart ? 1 : 0);
            if (!fromState.empty())
               query->And(CRuleTable::getStateColumnName(), CQUERY_OP_IN, fromState);

            adapters::CRuleAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *query);

            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CRule>> CRule::getRules(const std::string& interpreterName) const
         {
            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
                     From(CRuleTable::getTableName()).
                     Where(CRuleTable::getInterpreterColumnName(), CQUERY_OP_EQUAL, interpreterName);

            adapters::CRuleAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         boost::shared_ptr<entities::CRule> CRule::getRule(int ruleId) const
         {
            adapters::CRuleAdapter adapter;

            const auto qSelect = m_databaseRequester->newQuery();

            qSelect->Select().
                     From(CRuleTable::getTableName()).
                     Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleId);

            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().empty())
            {
               // Rule not found
               throw shared::exception::CEmptyResult((boost::format("Rule Id %1% not found in database") % ruleId).str());
            }
            return adapter.getResults().at(0);
         }

         int CRule::addRule(const entities::CRule& ruleData)
         {
            const auto qInsert = m_databaseRequester->newQuery();

            qInsert->InsertInto(CRuleTable::getTableName(),
                                CRuleTable::getNameColumnName(),
                                CRuleTable::getDescriptionColumnName(),
                                CRuleTable::getInterpreterColumnName(),
                                CRuleTable::getEditorColumnName(),
                                CRuleTable::getModelColumnName(),
                                CRuleTable::getContentColumnName(),
                                CRuleTable::getConfigurationColumnName(),
                                CRuleTable::getStateColumnName()).
                     Values(ruleData.Name(),
                            ruleData.Description(),
                            ruleData.Interpreter(),
                            ruleData.Editor(),
                            ruleData.Model(),
                            ruleData.Content(),
                            ruleData.Configuration(),
                            ruleData.State.isDefined() ? ruleData.State() : entities::ERuleState::kStopped);

            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");


            const auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CRuleTable::getIdColumnName()).
                     From(CRuleTable::getTableName()).
                     Where(CRuleTable::getNameColumnName(), CQUERY_OP_EQUAL, ruleData.Name()).
                     OrderBy(CRuleTable::getIdColumnName(), CQuery::kDesc);

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (!adapter.getResults().empty())
            {
               //search for inserted rule
               const auto createdId = adapter.getResults()[0];

               //update all optional flags 
               const auto qUpdate = m_databaseRequester->newQuery();

               //update error message
               if (ruleData.ErrorMessage.isDefined())
               {
                  qUpdate->Clear().Update(CRuleTable::getTableName()).
                           Set(CRuleTable::getErrorMessageColumnName(), ruleData.ErrorMessage()).
                           Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update error message field");
               }

               //update start date
               if (ruleData.StartDate.isDefined())
               {
                  qUpdate->Clear().Update(CRuleTable::getTableName()).
                           Set(CRuleTable::getStartDateColumnName(), ruleData.StartDate()).
                           Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update start date field");
               }

               //update stop date
               if (ruleData.StopDate.isDefined())
               {
                  qUpdate->Clear().Update(CRuleTable::getTableName()).
                           Set(CRuleTable::getStopDateColumnName(), ruleData.StopDate()).
                           Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update stop date field");
               }

               return createdId;
            }

            throw shared::exception::CEmptyResult("Cannot retrieve inserted Rule");
         }

         void CRule::updateRule(const entities::CRule& ruleData)
         {
            if (!ruleData.Id.isDefined())
               throw CDatabaseException("Need an id to update");

            const auto query = m_databaseRequester->newQuery();
            query->Update(CRuleTable::getTableName());

            if (ruleData.Name.isDefined())
               query->MultiSet(CRuleTable::getNameColumnName(), ruleData.Name());
            if (ruleData.Description.isDefined())
               query->MultiSet(CRuleTable::getDescriptionColumnName(), ruleData.Description());
            if (ruleData.Interpreter.isDefined())
               query->MultiSet(CRuleTable::getInterpreterColumnName(), ruleData.Interpreter());
            if (ruleData.Editor.isDefined())
               query->MultiSet(CRuleTable::getEditorColumnName(), ruleData.Editor());
            if (ruleData.Model.isDefined())
               query->MultiSet(CRuleTable::getModelColumnName(), ruleData.Model());
            if (ruleData.Content.isDefined())
               query->MultiSet(CRuleTable::getContentColumnName(), ruleData.Content());
            if (ruleData.Configuration.isDefined())
               query->MultiSet(CRuleTable::getConfigurationColumnName(), ruleData.Configuration());
            if (ruleData.AutoStart.isDefined())
               query->MultiSet(CRuleTable::getAutoStartColumnName(), ruleData.AutoStart());
            if (ruleData.State.isDefined())
               query->MultiSet(CRuleTable::getStateColumnName(), ruleData.State());
            if (ruleData.ErrorMessage.isDefined())
               query->MultiSet(CRuleTable::getErrorMessageColumnName(), ruleData.ErrorMessage());
            if (ruleData.StartDate.isDefined())
               query->MultiSet(CRuleTable::getStartDateColumnName(), ruleData.StartDate());
            if (ruleData.StopDate.isDefined())
               query->MultiSet(CRuleTable::getStopDateColumnName(), ruleData.StopDate());

            query->Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData.Id());

            if (m_databaseRequester->queryStatement(*query) <= 0)
               throw shared::exception::CEmptyResult("Fail to update rule");
         }

         void CRule::deleteRule(int ruleId)
         {
            const auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->DeleteFrom(CRuleTable::getTableName()).
                     Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         // [END] IRuleRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 
