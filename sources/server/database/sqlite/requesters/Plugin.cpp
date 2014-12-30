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

   CPlugin::CPlugin(boost::shared_ptr<CSQLiteRequester> & databaseRequester)
      :m_databaseRequester(databaseRequester)
   {
   }

   CPlugin::~CPlugin()
   {
   }

   // IPluginRequester implementation
   int CPlugin::addInstance(const entities::CPlugin& newPlugin)
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

      adapters::CSingleValueAdapter<int> adapter;
      m_databaseRequester->queryEntities<int>(&adapter, qSelect);
      if(adapter.getResults().size() >= 1)
         return adapter.getResults()[0];

      throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
   }

   boost::shared_ptr<entities::CPlugin> CPlugin::getInstance(int pluginId)
   {
      adapters::CPluginAdapter adapter;

      CQuery qSelect;

      qSelect.Select().
         From(CPluginTable::getTableName()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

      m_databaseRequester->queryEntities<const boost::shared_ptr<entities::CPlugin> >(&adapter, qSelect);
      if (adapter.getResults().empty())
      {
         // Plugin not found
         std::string sEx = (boost::format("Plugin Id %1% not found in database") % pluginId).str(); 
         throw shared::exception::CEmptyResult(sEx);
      }
      return adapter.getResults().at(0);
   }

   boost::shared_ptr<entities::CPlugin> CPlugin::getSystemInstance()
   {
      adapters::CPluginAdapter adapter;

      CQuery qSelect;

      qSelect.Select().
         From(CPluginTable::getTableName()).
         Where(CPluginTable::getCategoryColumnName(), CQUERY_OP_EQUAL, entities::EPluginCategory::kSystem);

      m_databaseRequester->queryEntities<const boost::shared_ptr<entities::CPlugin> >(&adapter, qSelect);
      if (adapter.getResults().empty())
      {
         // Plugin not found
         throw shared::exception::CEmptyResult("System plugin not found in database");
      }
      return adapter.getResults().at(0);
   }


   std::vector<const boost::shared_ptr<entities::CPlugin> > CPlugin::getInstances()
   {
      adapters::CPluginAdapter adapter;

      CQuery qSelect;
      qSelect.Select().From(CPluginTable::getTableName());

      m_databaseRequester->queryEntities<const boost::shared_ptr<entities::CPlugin> >(&adapter, qSelect);
      return adapter.getResults();
   }

   void CPlugin::updateInstance(const entities::CPlugin & updatedPluginData)
   {
      CQuery qUpdate;

      if(!updatedPluginData.Id.isDefined())
         throw CDatabaseException("Need an id to update");

      //update name
      if(updatedPluginData.Name.isDefined())
      {
         qUpdate.Clear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getNameColumnName(), updatedPluginData.Name()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update name");
      }

      //update configuration
      if(updatedPluginData.Configuration.isDefined())
      {
         qUpdate.Clear().Update(CPluginTable::getTableName()).
         Set(CPluginTable::getConfigurationColumnName(), updatedPluginData.Configuration()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update configuration");
      }
      
      //update autostart
      if(updatedPluginData.AutoStart.isDefined())
      {
         qUpdate.Clear().Update(CPluginTable::getTableName()).
            Set(CPluginTable::getAutoStartColumnName(), updatedPluginData.AutoStart()).
         Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

         if(m_databaseRequester->queryStatement(qUpdate) <= 0)
            throw CDatabaseException("Failed to update enabled field");
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

