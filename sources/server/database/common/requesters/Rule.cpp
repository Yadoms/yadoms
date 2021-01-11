#include "stdafx.h"
#include "Rule.h"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/DatabaseException.hpp"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CRule::CRule(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CRule::~CRule()
         {
         }

         // IRuleRequester implementation

         std::vector<boost::shared_ptr<entities::CRule>> CRule::getRules() const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().
               From(CRuleTable::getTableName());

            adapters::CRuleAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CRule>> CRule::getRules(const std::string& interpreterName) const
         {
            auto qSelect = m_databaseRequester->newQuery();
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

            auto qSelect = m_databaseRequester->newQuery();

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

         int CRule::addRule(boost::shared_ptr<const entities::CRule> ruleData)
         {
            auto qInsert = m_databaseRequester->newQuery();

            qInsert->InsertInto(CRuleTable::getTableName(),
                                CRuleTable::getNameColumnName(),
                                CRuleTable::getDescriptionColumnName(),
                                CRuleTable::getInterpreterColumnName(),
                                CRuleTable::getEditorColumnName(),
                                CRuleTable::getModelColumnName(),
                                CRuleTable::getContentColumnName(),
                                CRuleTable::getConfigurationColumnName(),
                                CRuleTable::getStateColumnName()).
               Values(ruleData->Name(),
                      ruleData->Description(),
                      ruleData->Interpreter(),
                      ruleData->Editor(),
                      ruleData->Model(),
                      ruleData->Content(),
                      ruleData->Configuration(),
                      ruleData->State.isDefined() ? ruleData->State() : entities::ERuleState::kStopped);

            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");


            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CRuleTable::getIdColumnName()).
               From(CRuleTable::getTableName()).
               Where(CRuleTable::getNameColumnName(), CQUERY_OP_EQUAL, ruleData->Name()).
               OrderBy(CRuleTable::getIdColumnName(), CQuery::kDesc);

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
            {
               //search for inserted rule
               auto createdId = adapter.getResults()[0];

               //update all optional flags 
               auto qUpdate = m_databaseRequester->newQuery();

               //update error message
               if (ruleData->ErrorMessage.isDefined())
               {
                  qUpdate->Clear().Update(CRuleTable::getTableName()).
                     Set(CRuleTable::getErrorMessageColumnName(), ruleData->ErrorMessage()).
                     Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update error message field");
               }

               //update start date
               if (ruleData->StartDate.isDefined())
               {
                  qUpdate->Clear().Update(CRuleTable::getTableName()).
                     Set(CRuleTable::getStartDateColumnName(), ruleData->StartDate()).
                     Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update start date field");
               }

               //update stop date
               if (ruleData->StopDate.isDefined())
               {
                  qUpdate->Clear().Update(CRuleTable::getTableName()).
                     Set(CRuleTable::getStopDateColumnName(), ruleData->StopDate()).
                     Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                     throw CDatabaseException("Failed to update stop date field");
               }

               return createdId;
            }

            throw shared::exception::CEmptyResult("Cannot retrieve inserted Rule");
         }

         void CRule::updateRule(boost::shared_ptr<const entities::CRule> ruleData)
         {
            auto qUpdate = m_databaseRequester->newQuery();

            if (!ruleData->Id.isDefined())
               throw CDatabaseException("Need an id to update");

            //update name
            if (ruleData->Name.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getNameColumnName(), ruleData->Name()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update name");
            }

            //update configuration
            if (ruleData->Description.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getDescriptionColumnName(), ruleData->Description()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update description");
            }

            //update interpreter
            if (ruleData->Interpreter.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getInterpreterColumnName(), ruleData->Interpreter()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update interpreter");
            }

            //update editor
            if (ruleData->Editor.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getEditorColumnName(), ruleData->Editor()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update editor");
            }

            //update model
            if (ruleData->Model.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getModelColumnName(), ruleData->Model()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update model");
            }

            //update content
            if (ruleData->Content.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getContentColumnName(), ruleData->Content()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update content field");
            }

            //update configuration
            if (ruleData->Configuration.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getConfigurationColumnName(), ruleData->Configuration()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update Configuration field");
            }

            //update autoStart flag
            if (ruleData->AutoStart.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getAutoStartColumnName(), ruleData->AutoStart()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update autoStart flag field");
            }

            //update state flag
            if (ruleData->State.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getStateColumnName(), ruleData->State()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update state flag field");
            }

            //update error message
            if (ruleData->ErrorMessage.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getErrorMessageColumnName(), ruleData->ErrorMessage()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update error message field");
            }

            //update start date
            if (ruleData->StartDate.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getStartDateColumnName(), ruleData->StartDate()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update start date field");
            }

            //update stop date
            if (ruleData->StopDate.isDefined())
            {
               qUpdate->Clear().Update(CRuleTable::getTableName()).
                  Set(CRuleTable::getStopDateColumnName(), ruleData->StopDate()).
                  Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update stop date field");
            }
         }

         void CRule::deleteRule(int ruleId)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->DeleteFrom(CRuleTable::getTableName()).
               Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         // [END] IRuleRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


