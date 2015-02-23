#include "stdafx.h"
#include "Rule.h"
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"
#include <shared/shared/exception/EmptyResult.hpp>
#include "../adapters/SingleValueAdapter.hpp"
#include "../../DatabaseException.hpp"


namespace database { namespace sqlite { namespace requesters { 

   CRule::CRule(boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CRule::~CRule()
   {
   }

   // IRuleRequester implementation

   std::vector<boost::shared_ptr<entities::CRule> > CRule::getRules() const
   {
      CQuery qSelect;
      qSelect. Select().
         From(CRuleTable::getTableName());

      adapters::CRuleAdapter adapter;
      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CRule> >(&adapter, qSelect);
      return adapter.getResults();
   }

   boost::shared_ptr<entities::CRule> CRule::getRule(int ruleId) const
   {
      adapters::CRuleAdapter adapter;

      CQuery qSelect;

      qSelect.Select().
         From(CRuleTable::getTableName()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleId);

      m_databaseRequester->queryEntities<boost::shared_ptr<entities::CRule> >(&adapter, qSelect);
      if (adapter.getResults().empty())
      {
         // Rule not found
         std::string sEx = (boost::format("Rule Id %1% not found in database") % ruleId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
      return adapter.getResults().at(0);
   }

   int CRule::addRule(boost::shared_ptr<const entities::CRule> ruleData)
   {
      CQuery qInsert;

      qInsert.InsertInto(CRuleTable::getTableName(), CRuleTable::getNameColumnName(), CRuleTable::getDescriptionColumnName(), CRuleTable::getTypeColumnName(), CRuleTable::getModelColumnName(), CRuleTable::getContentColumnName(), CRuleTable::getConfigurationColumnName(), CRuleTable::getStateColumnName()).
         Values(ruleData->Name(), 
         ruleData->Description(),
         ruleData->Type(),
         ruleData->Model(),
         ruleData->Content(),
         ruleData->Configuration(),
         ruleData->State());

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");


      CQuery qSelect;
      qSelect. Select(CRuleTable::getIdColumnName()).
         From(CRuleTable::getTableName()).
         Where(CRuleTable::getNameColumnName(), CQUERY_OP_EQUAL, ruleData->Name()).
         OrderBy(CRuleTable::getIdColumnName(), CQUERY_ORDER_DESC);

      adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if (adapter.getResults().size() >= 1)
      {
         //search for inserted rule
         int createdId = adapter.getResults()[0];

         //update all optional flags 
         CQuery qUpdate;

         //update error message
         if (ruleData->ErrorMessage.isDefined())
         {
            qUpdate.Clear().Update(CRuleTable::getTableName()).
               Set(CRuleTable::getErrorMessageColumnName(), ruleData->ErrorMessage()).
               Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw CDatabaseException("Failed to update error message field");
         }

         //update start date
         if (ruleData->StartDate.isDefined())
         {
            qUpdate.Clear().Update(CRuleTable::getTableName()).
               Set(CRuleTable::getStartDateColumnName(), ruleData->StartDate()).
               Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw CDatabaseException("Failed to update start date field");
         }

         //update stop date
         if (ruleData->StopDate.isDefined())
         {
            qUpdate.Clear().Update(CRuleTable::getTableName()).
               Set(CRuleTable::getStopDateColumnName(), ruleData->StopDate()).
               Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw CDatabaseException("Failed to update stop date field");
         }

         return createdId;
      }

      throw shared::exception::CEmptyResult("Cannot retrieve inserted Rule");
   }
   
   void CRule::updateRule(boost::shared_ptr<const entities::CRule> ruleData)
   {
      CQuery qUpdate;

      if(!ruleData->Id.isDefined())
         throw CDatabaseException("Need an id to update");

      //update name
      if(ruleData->Name.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
         Set(CRuleTable::getNameColumnName(), ruleData->Name()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update name");
      }

      //update configuration
      if(ruleData->Description.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
         Set(CRuleTable::getDescriptionColumnName(), ruleData->Description()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update description");
      }
      
      //update content
      if(ruleData->Content.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getContentColumnName(), ruleData->Content()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update content field");
      }
      
      //update configuration
      if(ruleData->Configuration.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getConfigurationColumnName(), ruleData->Configuration()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update Configuration field");
      }
      
      //update enable flag
      if(ruleData->Enabled.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getEnabledColumnName(), ruleData->Enabled()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update enable flag field");
      }   
      
      //update state flag
      if(ruleData->State.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getStateColumnName(), ruleData->State()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update state flag field");
      }   
      
      //update error message
      if(ruleData->ErrorMessage.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getErrorMessageColumnName(), ruleData->ErrorMessage()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update error message field");
      }

      //update start date
      if(ruleData->StartDate.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getStartDateColumnName(), ruleData->StartDate()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update start date field");
      }   
      
      //update stop date
      if(ruleData->StopDate.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getStopDateColumnName(), ruleData->StopDate()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update stop date field");
      }
   }

   void CRule::deleteRule(int ruleId)
   {
      CQuery qUpdate;
      qUpdate. DeleteFrom(CRuleTable::getTableName()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   // [END] IRuleRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

