#include "stdafx.h"
#include "Device.h"
#include <shared/exception/EmptyResult.hpp>
#include "database/common/adapters/DatabaseAdapters.h"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "database/common/DatabaseTables.h"
#include "database/common/Query.h"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CDevice::CDevice(boost::shared_ptr<IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CDevice::~CDevice()
         {
         }

         bool CDevice::deviceExists(const int deviceId) const
         {
            try
            {
               getDevice(deviceId, true);
            }
            catch (shared::exception::CEmptyResult&)
            {
               return false;
            }
            return true;
         }

         bool CDevice::deviceExists(const int pluginId, const std::string& deviceName) const
         {
            try
            {
               getDeviceInPlugin(pluginId, deviceName, true);
            }
            catch (shared::exception::CEmptyResult&)
            {
               return false;
            }
            return true;
         }

         boost::shared_ptr<entities::CDevice> CDevice::getDevice(int deviceId, bool blacklistedIncluded) const
         {
            auto qSelect = m_databaseRequester->newQuery();

            if (blacklistedIncluded)
               qSelect.Select().
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
            else
               qSelect.Select().
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId).
                      And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
               throw shared::exception::CEmptyResult((boost::format("Cannot retrieve Device Id=%1% in database") % deviceId).str());

            return adapter.getResults()[0];
         }

         boost::shared_ptr<entities::CDevice> CDevice::getDeviceInPlugin(int pluginId, const std::string& name, bool blacklistedIncluded) const
         {
            //search for such a device
            auto qSelect = m_databaseRequester->newQuery();

            if (blacklistedIncluded)
               qSelect.Select().
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
                      And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, name);
            else
               qSelect.Select().
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
                      And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, name).
                      And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
               throw shared::exception::CEmptyResult((boost::format("Cannot retrieve Device Id=%1% in database") % name).str());

            return adapter.getResults()[0];
         }

         std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevicesIdFromFriendlyName(const std::string& friendlyName) const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(CDeviceTable::getTableName()).
                   Where(CDeviceTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, friendlyName).
                   And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDeviceWithCapacity(const std::string& capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode) const
         {
            auto subQuery = m_databaseRequester->newQuery();
            subQuery.Select(CKeywordTable::getDeviceIdColumnName()).
                    From(CKeywordTable::getTableName()).
                    Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName);

            if (accessMode() == shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
            {
               //we add a constraint on accessmode
               subQuery.And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, accessMode);
            }

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(CDeviceTable::getTableName()).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, subQuery).
                   And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode, const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const
         {
            auto subQuery = m_databaseRequester->newQuery();
            subQuery.Select(CKeywordTable::getDeviceIdColumnName()).
                    From(CKeywordTable::getTableName()).
                    Where(CKeywordTable::getTypeColumnName(), CQUERY_OP_EQUAL, capacityType);

            if (capacityAccessMode() == shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
            {
               //we add a constraint on accessmode
               subQuery.And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, capacityAccessMode);
            }

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(CDeviceTable::getTableName()).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, subQuery).
                   And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }


         boost::shared_ptr<entities::CDevice> CDevice::createDevice(int pluginId, const std::string& name, const std::string& friendlyName, const std::string& type, const std::string& model, const shared::CDataContainer& details)
         {
            if (deviceExists(pluginId, name))
               throw shared::exception::CEmptyResult("The device already exists, cannot create it a new time");

            //device not found, creation is enabled

            //get a good name
            auto realFriendlyName = friendlyName;
            if (realFriendlyName == std::string())
               realFriendlyName = name;

            //insert in db
            auto qInsert = m_databaseRequester->newQuery();
            qInsert.InsertInto(CDeviceTable::getTableName(),
                               CDeviceTable::getPluginIdColumnName(),
                               CDeviceTable::getNameColumnName(),
                               CDeviceTable::getFriendlyNameColumnName(),
                               CDeviceTable::getTypeColumnName(),
                               CDeviceTable::getModelColumnName(),
                               CDeviceTable::getDetailsColumnName()).
                   Values(pluginId,
                          name,
                          realFriendlyName,
                          type,
                          model,
                          details.serialize());
            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("Fail to insert new device");

            //device is created, just find it in table and return entity
            auto deviceCreated = getDeviceInPlugin(pluginId, name);
            if (!deviceCreated)
               throw shared::exception::CEmptyResult("Fail to retrieve created device");

            return deviceCreated;
         }

         void CDevice::updateDeviceFriendlyName(int deviceId, const std::string& newFriendlyName)
         {
            if (!deviceExists(deviceId))
               throw shared::exception::CEmptyResult("The device does not exists");

            //device not found, creation is enabled

            //get a good name
            if (newFriendlyName != std::string())
            {
               //insert in db
               auto qUpdate = m_databaseRequester->newQuery();
               qUpdate.Update(CDeviceTable::getTableName()).
                      Set(CDeviceTable::getFriendlyNameColumnName(), newFriendlyName).
                      Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("Fail to update device friendlyName");
            }
         }

         void CDevice::updateDeviceConfiguration(int deviceId, const shared::CDataContainer& configuration)
         {
            if (!deviceExists(deviceId))
               throw shared::exception::CEmptyResult("The device does not exists");

            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(CDeviceTable::getTableName()).
                   Set(CDeviceTable::getConfigurationColumnName(), configuration.serialize()).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update device configuration");
         }

         void CDevice::updateDeviceDetails(int deviceId, const shared::CDataContainer& details)
         {
            if (!deviceExists(deviceId))
               throw shared::exception::CEmptyResult("The device does not exists");

            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(CDeviceTable::getTableName()).
                   Set(CDeviceTable::getDetailsColumnName(), details.serialize()).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update device details");
         }

         void CDevice::updateDeviceModel(int deviceId, const std::string& model)
         {
            if (!deviceExists(deviceId))
               throw shared::exception::CEmptyResult("The device does not exists");

            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(CDeviceTable::getTableName()).
                   Set(CDeviceTable::getModelColumnName(), model).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update device model");
         }

         void CDevice::updateDeviceType(int deviceId, const std::string& type)
         {
            if (!deviceExists(deviceId))
               throw shared::exception::CEmptyResult("The device does not exists");

            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(CDeviceTable::getTableName()).
                   Set(CDeviceTable::getTypeColumnName(), type).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update device type");
         }

         void CDevice::updateDeviceBlacklistState(int deviceId, const bool blacklist)
         {
            if (!deviceExists(deviceId))
               throw shared::exception::CEmptyResult("The device does not exists");

            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(CDeviceTable::getTableName()).
                   Set(CDeviceTable::getBlacklistColumnName(), blacklist).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update device blacklist");
         }

         bool CDevice::isDeviceBlacklisted(int deviceId) const
         {
            const auto& device = getDevice(deviceId, true);
            return device->Blacklist();
         }

         std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevices(bool blacklistedIncluded) const
         {
            auto qSelect = m_databaseRequester->newQuery();

            if (blacklistedIncluded)
               qSelect.Select().
                      From(CDeviceTable::getTableName());
            else
               qSelect.Select().
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<std::string> CDevice::getDevicesNames(int pluginId,
                                                           bool blacklistedIncluded) const
         {
            auto qSelect = m_databaseRequester->newQuery();

            if (blacklistedIncluded)
               qSelect.Select(CDeviceTable::getNameColumnName()).
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);
            else
               qSelect.Select(CDeviceTable::getNameColumnName()).
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
                      And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CSingleValueAdapter<std::string> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevices(int pluginId,
                                                                               bool blacklistedIncluded) const
         {
            auto qSelect = m_databaseRequester->newQuery();

            if (blacklistedIncluded)
               qSelect.Select().
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);
            else
               qSelect.Select().
                      From(CDeviceTable::getTableName()).
                      Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
                      And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         void CDevice::removeDevice(int deviceId)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(CDeviceTable::getTableName()).
                   Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
            if (m_databaseRequester->queryStatement(qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            qDelete.Clear().DeleteFrom(CKeywordTable::getTableName()).
                   Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
            m_databaseRequester->queryStatement(qDelete);
         }

         void CDevice::removeDevice(int pluginId, const std::string& deviceName)
         {
            removeDevice(getDeviceInPlugin(pluginId, deviceName)->Id());
         }

         void CDevice::removeAllDeviceForPlugin(int pluginId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(CDeviceTable::getTableName()).Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            auto devicestoDelete = adapter.getResults();

            for (auto i = devicestoDelete.begin(); i != devicestoDelete.end(); ++i)
            {
               removeDevice((*i)->Id());
            }
         }

         // [END] IDeviceRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


