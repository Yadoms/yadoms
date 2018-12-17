#include "stdafx.h"
#include "Plugin.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"
#include "database/DatabaseException.hpp"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CPlugin::CPlugin(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CPlugin::~CPlugin()
         {
         }

         // IPluginRequester implementation
         int CPlugin::addInstance(const entities::CPlugin& newPlugin)
         {
            auto qInsert = m_databaseRequester->newQuery();

            qInsert->InsertInto(CPluginTable::getTableName(), CPluginTable::getDisplayNameColumnName(), CPluginTable::getTypeColumnName(), CPluginTable::getConfigurationColumnName(), CPluginTable::getAutoStartColumnName()).
               Values(newPlugin.DisplayName(),
                      newPlugin.Type(),
                      newPlugin.Configuration(),
                      newPlugin.AutoStart());

            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");


            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select(CPluginTable::getIdColumnName()).
               From(CPluginTable::getTableName()).
               Where(CPluginTable::getDisplayNameColumnName(), CQUERY_OP_EQUAL, newPlugin.DisplayName()).
               And(CPluginTable::getTypeColumnName(), CQUERY_OP_EQUAL, newPlugin.Type()).
               OrderBy(CPluginTable::getIdColumnName(), CQuery::kDesc);

            adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
         }

         boost::shared_ptr<entities::CPlugin> CPlugin::getInstance(int pluginId)
         {
            adapters::CPluginAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();

            qSelect->Select().
               From(CPluginTable::getTableName()).
               Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().empty())
               throw shared::exception::CEmptyResult((boost::format("Plugin Id %1% not found in database") % pluginId).str());

            return adapter.getResults().at(0);
         }

         boost::shared_ptr<entities::CPlugin> CPlugin::getSystemInstance()
         {
            adapters::CPluginAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();

            qSelect->Select().
               From(CPluginTable::getTableName()).
               Where(CPluginTable::getCategoryColumnName(), CQUERY_OP_EQUAL, entities::EPluginCategory::kSystem);

            m_databaseRequester->queryEntities(&adapter, *qSelect);
            if (adapter.getResults().empty())
            {
               // Plugin not found
               throw shared::exception::CEmptyResult("System plugin not found in database");
            }
            return adapter.getResults().at(0);
         }


         std::vector<boost::shared_ptr<entities::CPlugin>> CPlugin::getInstances()
         {
            adapters::CPluginAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();
            qSelect->Select().From(CPluginTable::getTableName());

            m_databaseRequester->queryEntities(&adapter, *qSelect);
            return adapter.getResults();
         }

         void CPlugin::updateInstance(const entities::CPlugin& updatedPluginData)
         {
            auto qUpdate = m_databaseRequester->newQuery();

            if (!updatedPluginData.Id.isDefined())
               throw CDatabaseException("Need an id to update");

            //update name
            if (updatedPluginData.DisplayName.isDefined())
            {
               qUpdate->Clear().Update(CPluginTable::getTableName()).
                  Set(CPluginTable::getDisplayNameColumnName(), updatedPluginData.DisplayName()).
                  Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update name");
            }

            //update configuration
            if (updatedPluginData.Configuration.isDefined())
            {
               qUpdate->Clear().Update(CPluginTable::getTableName()).
                  Set(CPluginTable::getConfigurationColumnName(), updatedPluginData.Configuration()).
                  Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update configuration");
            }

            //update autostart
            if (updatedPluginData.AutoStart.isDefined())
            {
               qUpdate->Clear().Update(CPluginTable::getTableName()).
                  Set(CPluginTable::getAutoStartColumnName(), updatedPluginData.AutoStart()).
                  Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

               if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
                  throw CDatabaseException("Failed to update enabled field");
            }
         }

         void CPlugin::removeInstance(int pluginId)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->DeleteFrom(CPluginTable::getTableName()).
               Where(CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

            if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CPlugin::disableAutoStartForAllPluginInstances(const std::string& pluginName)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate->Update(CPluginTable::getTableName()).
               Set(CPluginTable::getAutoStartColumnName(), false).
               Where(CPluginTable::getTypeColumnName(), CQUERY_OP_EQUAL, pluginName);

            m_databaseRequester->queryStatement(*qUpdate);
         }

         // [END] IPluginRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


