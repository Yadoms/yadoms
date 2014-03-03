#include "stdafx.h"
#include "SQLitePluginRequester.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"
#include "database/DatabaseException.hpp"

namespace database { namespace sqlite { namespace requesters { 

   CSQLitePluginRequester::CSQLitePluginRequester(const CSQLiteDataProvider & databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CSQLitePluginRequester::~CSQLitePluginRequester()
   {
   }

   // IPluginRequester implementation
   int CSQLitePluginRequester::addPlugin(boost::shared_ptr<database::entities::CPlugin> newPlugin)
   {
      CQuery qInsert;

      qInsert.InsertInto(CPluginTable::getTableName(), CPluginTable::getNameColumnName(), CPluginTable::getPluginNameColumnName(), CPluginTable::getConfigurationColumnName(), CPluginTable::getEnabledColumnName() ).
         Values(newPlugin->getName(), 
         newPlugin->getPluginName(),
         newPlugin->getConfiguration(), 
         newPlugin->getEnabled());

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");


      CQuery qSelect;
      qSelect. Select(CPluginTable::getIdColumnName()).
         From(CPluginTable::getTableName()).
         Where(CPluginTable::getNameColumnName(), CQUERY_OP_EQUAL, newPlugin->getName()).
         And(CPluginTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, newPlugin->getPluginName()).
         OrderBy(CPluginTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
   }

   boost::shared_ptr<database::entities::CPlugin> CSQLitePluginRequester::getPlugin(int pluginId)
   {
      database::sqlite::adapters::CPluginAdapter adapter;

      CQuery qSelect;

      qSelect. Select().
         From(CPluginTable::getTableName()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId).
         And(CPluginTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false);

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CPlugin> >(&adapter, qSelect);
      if (adapter.getResults().empty())
      {
         // Plugin not found
         std::string sEx = (boost::format("Plugin Id %1% not found in database") % pluginId).str(); 
         throw shared::exception::CException(sEx);
      }
      return adapter.getResults().at(0);
   }

   std::vector<boost::shared_ptr<database::entities::CPlugin> > CSQLitePluginRequester::getPlugins(bool evenDeleted)
   {
      database::sqlite::adapters::CPluginAdapter adapter;

      CQuery qSelect;
      qSelect.Select().From(CPluginTable::getTableName());

      if (!evenDeleted)
         qSelect.Where(CPluginTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false);

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CPlugin> >(&adapter, qSelect);
      return adapter.getResults();
   }

   //test
   std::vector<std::string> CSQLitePluginRequester::getPluginNameList()
   {
      database::sqlite::adapters::CSingleValueAdapter<std::string> adapter;

      CQuery qSelect;
      qSelect. Select(CPluginTable::getNameColumnName()).
         From(CPluginTable::getTableName()).
         Where(CPluginTable::getDeletedColumnName(), CQUERY_OP_EQUAL, false).
         OrderBy(CPluginTable::getNameColumnName());

      m_databaseRequester->queryEntities<std::string>(&adapter, qSelect);
      return adapter.getResults();
   }

   void CSQLitePluginRequester::updatePluginConfiguration(int pluginId, const std::string& newConfiguration)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CPluginTable::getConfigurationColumnName(), newConfiguration).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLitePluginRequester::updatePlugin(const database::entities::CPlugin & updatedPluginData)
   {
      CQuery qUpdate;

      if(!updatedPluginData.isIdFilled())
         throw database::CDatabaseException("Need an id to update");

      //update pluginName
      if(updatedPluginData.isPluginNameFilled())
      {
         qUpdate.CLear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getPluginNameColumnName(), updatedPluginData.getPluginName()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update pluginName");
      }

      //update name
      if(updatedPluginData.isNameFilled())
      {
         qUpdate.CLear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getNameColumnName(), updatedPluginData.getName()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update name");
      }

      //update configuration
      if(updatedPluginData.isConfigurationFilled())
      {
         qUpdate.CLear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getConfigurationColumnName(), updatedPluginData.getConfiguration()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update configuration");
      }
      
      //update deleted
      if(updatedPluginData.isDeletedFilled())
      {
         qUpdate.CLear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getDeletedColumnName(), updatedPluginData.getDeleted()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update deleted field");
      }
      
      //update enabled
      if(updatedPluginData.isEnabledFilled())
      {
         qUpdate.CLear().Update(CPluginTable::getTableName()).
            Set(CPluginTable::getEnabledColumnName(), updatedPluginData.getEnabled()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.getId());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update enabled field");
      }
      

   }

   void CSQLitePluginRequester::removePlugin(int pluginId)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CPluginTable::getDeletedColumnName(), true).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLitePluginRequester::enableInstance(int pluginId, bool enable)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CPluginTable::getEnabledColumnName(), enable).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CSQLitePluginRequester::disableAllPluginInstance(const std::string& pluginName)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CPluginTable::getEnabledColumnName(), false).
         Where(CPluginTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName);

      m_databaseRequester->queryStatement(qUpdate);
   }

   // [END] IPluginRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

