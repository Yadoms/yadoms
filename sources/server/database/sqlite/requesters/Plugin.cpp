#include "stdafx.h"
#include "Plugin.h"
#include <shared/exception/NotImplemented.hpp>
#include "database/sqlite/SQLiteDataProvider.h"
#include "database/sqlite/adapters/SingleValueAdapter.hpp"
#include "database/sqlite/adapters/SQLiteDatabaseAdapters.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/sqlite/SQLiteDatabaseTables.h"
#include "database/sqlite/Query.h"
#include "database/DatabaseException.hpp"

namespace database { namespace sqlite { namespace requesters { 

   CPlugin::CPlugin(CSQLiteDataProvider * databaseHandler, boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseHandler(databaseHandler), m_databaseRequester(databaseRequester)
   {
   }

   CPlugin::~CPlugin()
   {
   }

   // IPluginRequester implementation
   int CPlugin::addInstance(const database::entities::CPlugin& newPlugin)
   {
      CQuery qInsert;

      qInsert.InsertInto(CPluginTable::getTableName(), CPluginTable::getNameColumnName(), CPluginTable::getTypeColumnName(), CPluginTable::getConfigurationColumnName(), CPluginTable::getAutoStartColumnName() ).
         Values(newPlugin.Name(), 
         newPlugin.Type(),
         newPlugin.Configuration(), 
         newPlugin.AutoStart());

      if(m_databaseRequester->queryStatement(qInsert) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");


      CQuery qSelect;
      qSelect. Select(CPluginTable::getIdColumnName()).
         From(CPluginTable::getTableName()).
         Where(CPluginTable::getNameColumnName(), CQUERY_OP_EQUAL, newPlugin.Name()).
         And(CPluginTable::getTypeColumnName(), CQUERY_OP_EQUAL, newPlugin.Type()).
         OrderBy(CPluginTable::getIdColumnName(), CQUERY_ORDER_DESC);

      database::sqlite::adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];
      else
         throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
   }

   boost::shared_ptr<database::entities::CPlugin> CPlugin::getInstance(int pluginId)
   {
      database::sqlite::adapters::CPluginAdapter adapter;

      CQuery qSelect;

      qSelect.Select().
         From(CPluginTable::getTableName()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CPlugin> >(&adapter, qSelect);
      if (adapter.getResults().empty())
      {
         // Plugin not found
         std::string sEx = (boost::format("Plugin Id %1% not found in database") % pluginId).str(); 
         throw shared::exception::CException(sEx);
      }
      return adapter.getResults().at(0);
   }

   std::vector<boost::shared_ptr<database::entities::CPlugin> > CPlugin::getInstances()
   {
      database::sqlite::adapters::CPluginAdapter adapter;

      CQuery qSelect;
      qSelect.Select().From(CPluginTable::getTableName());

      m_databaseRequester->queryEntities<boost::shared_ptr<database::entities::CPlugin> >(&adapter, qSelect);
      return adapter.getResults();
   }

   //test
   std::vector<std::string> CPlugin::getPluginNameList()
   {
      database::sqlite::adapters::CSingleValueAdapter<std::string> adapter;

      CQuery qSelect;
      qSelect. Select(CPluginTable::getNameColumnName()).
         From(CPluginTable::getTableName()).
         OrderBy(CPluginTable::getNameColumnName());

      m_databaseRequester->queryEntities<std::string>(&adapter, qSelect);
      return adapter.getResults();
   }

   void CPlugin::updateInstance(const database::entities::CPlugin & updatedPluginData)
   {
      CQuery qUpdate;

      if(!updatedPluginData.Id.isDefined())
         throw database::CDatabaseException("Need an id to update");

      //update name
      if(updatedPluginData.Name.isDefined())
      {
         qUpdate.Clear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getNameColumnName(), updatedPluginData.Name()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update name");
      }

      //update configuration
      if(updatedPluginData.Configuration.isDefined())
      {
         qUpdate.Clear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getConfigurationColumnName(), updatedPluginData.Configuration()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update configuration");
      }
      
      //update autostart
      if(updatedPluginData.AutoStart.isDefined())
      {
         qUpdate.Clear().Update(CPluginTable::getTableName()).
            Set(CPluginTable::getAutoStartColumnName(), updatedPluginData.AutoStart()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw database::CDatabaseException("Failed to update enabled field");
      }
   }

   void CPlugin::removeInstance(int pluginId)
   {
      CQuery qUpdate;
      qUpdate. DeleteFrom(CPluginTable::getTableName()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      if(m_databaseRequester->queryStatement(qUpdate) <= 0)
         throw shared::exception::CEmptyResult("No lines affected");
   }

   void CPlugin::disableAutoStartForAllPluginInstances(const std::string& pluginName)
   {
      CQuery qUpdate;
      qUpdate. Update(CPluginTable::getTableName()).
         Set(CPluginTable::getAutoStartColumnName(), false).
         Where(CPluginTable::getTypeColumnName(), CQUERY_OP_EQUAL, pluginName);

      m_databaseRequester->queryStatement(qUpdate);
   }

   // [END] IPluginRequester implementation

} //namespace requesters
} //namespace sqlite
} //namespace database 

