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

   int CRule::addRule(const entities::CRule& data)
   {
      CQuery qInsert;

      qInsert.InsertInto(CRuleTable::getTableName(), CRuleTable::getNameColumnName(), CRuleTable::getDescriptionColumnName(), CRuleTable::getTypeColumnName(), CRuleTable::getModelColumnName(), CRuleTable::getContentColumnName(), CRuleTable::getConfigurationColumnName()).
         Values(data.Name(), 
         data.Description(),
         data.Type(),
         data.Model(),
         data.Content(),
         data.Configuration());

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");


      CQuery qSelect;
      qSelect. Select(CRuleTable::getIdColumnName()).
         From(CRuleTable::getTableName()).
         Where(CRuleTable::getNameColumnName(), CQUERY_OP_EQUAL, data.Name()).
         OrderBy(CRuleTable::getIdColumnName(), CQUERY_ORDER_DESC);

      adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if (adapter.getResults().size() >= 1)
         return adapter.getResults()[0];

      throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
   }
   
   void CRule::updateRule(const entities::CRule& data)
   {
      CQuery qUpdate;

      if(!data.Id.isDefined())
         throw CDatabaseException("Need an id to update");

      //update name
      if(data.Name.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
         Set(CRuleTable::getNameColumnName(), data.Name()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, data.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update name");
      }

      //update configuration
      if(data.Description.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
         Set(CRuleTable::getDescriptionColumnName(), data.Description()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, data.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update description");
      }
      
      //update content
      if(data.Content.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getContentColumnName(), data.Content()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, data.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update content field");
      }
      
      //update configuration
      if(data.Configuration.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getConfigurationColumnName(), data.Configuration()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, data.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update Configuration field");
      }
      
      //update enable flag
      if(data.Enable.isDefined())
      {
         qUpdate.Clear().Update(CRuleTable::getTableName()).
            Set(CRuleTable::getEnableColumnName(), data.Enable()).
         Where(CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, data.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update enable flag field");
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

