#include "stdafx.h"
#include "Device.h"
#include <shared/exception/EmptyResult.hpp>
#include <utility>
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
				: m_databaseRequester(std::move(databaseRequester))
			{
			}

			bool CDevice::deviceExists(int deviceId) const
			{
				try
				{
					const auto f = getDevice(deviceId, true);
					return f && f.get() != nullptr;
				}
				catch (shared::exception::CEmptyResult&)
				{
					return false;
				}
			}

			bool CDevice::deviceExists(int pluginId, const std::string& deviceName) const
			{
				try
				{
					const auto f = getDeviceInPlugin(pluginId, deviceName, true);
					return f && f.get() != nullptr;
				}
				catch (shared::exception::CEmptyResult&)
				{
					return false;
				}
			}

         boost::shared_ptr<entities::CDevice> CDevice::getDevice(int deviceId,
                                                                 bool blacklistedIncluded) const
			{
            const auto qSelect = m_databaseRequester->newQuery();

				if (blacklistedIncluded)
					qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
				else
					qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				if (adapter.getResults().empty())
					throw shared::exception::CEmptyResult(std::string("Cannot retrieve Device Id=" + std::to_string(deviceId) + " in database"));

				return adapter.getResults()[0];
			}

         boost::shared_ptr<entities::CDevice> CDevice::getDeviceInPlugin(int pluginId,
                                                                         const std::string& name,
                                                                         bool blacklistedIncluded) const
			{
				//search for such a device
            const auto qSelect = m_databaseRequester->newQuery();

				if (blacklistedIncluded)
					qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
					And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, name);
				else
					qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
					And(CDeviceTable::getNameColumnName(), CQUERY_OP_EQUAL, name).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				if (adapter.getResults().empty())
					throw shared::exception::CEmptyResult(std::string("Cannot retrieve Device Id=")
														  + name
														  + " in database");

				return adapter.getResults()[0];
			}

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevicesIdFromFriendlyName(const std::string& friendlyName) const
			{
            const auto qSelect = m_databaseRequester->newQuery();
				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, friendlyName).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDeviceWithCapacity(const std::string& capacityName,
																							 const shared::plugin::yPluginApi::EKeywordAccessMode&
																							 accessMode) const
			{
            const auto subQuery = m_databaseRequester->newQuery();
				subQuery->Select(CKeywordTable::getDeviceIdColumnName()).
					From(CKeywordTable::getTableName()).
					Where(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName);

				if (accessMode() == shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
				{
					//we add a constraint on accessmode
					subQuery->And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, accessMode);
				}

            const auto qSelect = m_databaseRequester->newQuery();
				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, *subQuery).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDeviceWithCapacityType(
				const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode,
				const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const
			{
            const auto subQuery = m_databaseRequester->newQuery();
				subQuery->Select(CKeywordTable::getDeviceIdColumnName()).
					From(CKeywordTable::getTableName()).
					Where(CKeywordTable::getTypeColumnName(), CQUERY_OP_EQUAL, capacityType);

				if (capacityAccessMode() == shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
				{
					//we add a constraint on accessmode
					subQuery->And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, capacityAccessMode);
				}

            const auto qSelect = m_databaseRequester->newQuery();
				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, *subQuery).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}


			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDeviceWithKeywordAccessMode(
				const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const
			{
            const auto subQuery = m_databaseRequester->newQuery();
				subQuery->Select(CKeywordTable::getDeviceIdColumnName()).
					From(CKeywordTable::getTableName());

				if (capacityAccessMode() == shared::plugin::yPluginApi::EKeywordAccessMode::kGetSet)
				{
					//we add a constraint on accessmode
					subQuery->Where(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, capacityAccessMode);
				}

            const auto qSelect = m_databaseRequester->newQuery();
				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, *subQuery).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}


			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDeviceWithKeywordHistoryDepth(
				const shared::plugin::yPluginApi::EHistoryDepth& historyDepth) const
			{
            const auto subQuery = m_databaseRequester->newQuery();
				subQuery->Select(CKeywordTable::getDeviceIdColumnName()).
					From(CKeywordTable::getTableName());

				if (historyDepth() != shared::plugin::yPluginApi::EHistoryDepth::kDefault)
				{
					//we add a constraint on history depth
					subQuery->Where(CKeywordTable::getHistoryDepthColumnName(), CQUERY_OP_EQUAL, historyDepth);
				}

            const auto qSelect = m_databaseRequester->newQuery();
				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, *subQuery).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

         CQuery& CDevice::addDevicesQueryFilters(CQuery& query,
                                                 const boost::optional<int>& deviceId,
                                                 const boost::optional<int>& pluginInstanceId,
                                                 const boost::optional<std::string>& friendlyName,
                                                 const boost::optional<std::string>& type,
                                                 const boost::optional<std::string>& model,
                                                 const std::set<std::string>& containsKeywordWithCapacityName,
                                                 const boost::optional<shared::plugin::yPluginApi::EKeywordAccessMode>&
                                                 containsKeywordWithAccessMode,
                                                 const std::set<shared::plugin::yPluginApi::EKeywordDataType>&
                                                 containsKeywordWithDataType,
                                                 const boost::optional<shared::plugin::yPluginApi::EHistoryDepth>&
                                                 containsKeywordWithHistoryDepth,
                                                 bool blacklistedIncluded) const
         {
            if (deviceId)
               query.And(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, *deviceId);
            if (pluginInstanceId)
               query.And(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, *pluginInstanceId);
            if (friendlyName)
               query.And(CDeviceTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, *friendlyName);
            if (type)
               query.And(CDeviceTable::getTypeColumnName(), CQUERY_OP_EQUAL, *type);
            if (model)
               query.And(CDeviceTable::getModelColumnName(), CQUERY_OP_EQUAL, *model);
            if (blacklistedIncluded)
               query.And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, blacklistedIncluded ? 1 : 0);

            if (!containsKeywordWithCapacityName.empty()
               || containsKeywordWithAccessMode
               || !containsKeywordWithDataType.empty()
               || containsKeywordWithHistoryDepth)
            {
               const auto subQuery = m_databaseRequester->newQuery();

               subQuery->Select(CKeywordTable::getDeviceIdColumnName()).
                         From(CKeywordTable::getTableName()).
                         WhereTrue();

               if (!containsKeywordWithCapacityName.empty())
                  subQuery->And(CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_IN, containsKeywordWithCapacityName);
               if (containsKeywordWithAccessMode)
                  subQuery->And(CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL,
                                *containsKeywordWithAccessMode);
               if (!containsKeywordWithDataType.empty())
                  subQuery->And(CKeywordTable::getTypeColumnName(), CQUERY_OP_IN, containsKeywordWithDataType);
               if (containsKeywordWithHistoryDepth)
                  subQuery->And(CKeywordTable::getHistoryDepthColumnName(), CQUERY_OP_EQUAL, *containsKeywordWithHistoryDepth);

               query.And(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, *subQuery);
            }

            return query;
         }

         std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevices(
            const boost::optional<int>& deviceId,
            const boost::optional<int>& pluginInstanceId,
            const boost::optional<std::string>& friendlyName,
            const boost::optional<std::string>& type,
            const boost::optional<std::string>& model,
            const std::set<std::string>& containsKeywordWithCapacityName,
            const boost::optional<shared::plugin::yPluginApi::EKeywordAccessMode>& containsKeywordWithAccessMode,
            const std::set<shared::plugin::yPluginApi::EKeywordDataType>& containsKeywordWithDataType,
            const boost::optional<shared::plugin::yPluginApi::EHistoryDepth>& containsKeywordWithHistoryDepth,
            bool blacklistedIncluded) const
         {
            const auto query = m_databaseRequester->newQuery();

            query->Select().
                   From(CDeviceTable::getTableName()).
                   WhereTrue();

            addDevicesQueryFilters(*query,
                                   deviceId,
                                   pluginInstanceId,
                                   friendlyName,
                                   type,
                                   model,
                                   containsKeywordWithCapacityName,
                                   containsKeywordWithAccessMode,
                                   containsKeywordWithDataType,
                                   containsKeywordWithHistoryDepth,
                                   blacklistedIncluded);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *query);

            return adapter.getResults();
         }

         void CDevice::getDevices(const boost::optional<int>& deviceId,
                                  const boost::optional<int>& pluginInstanceId,
                                  const boost::optional<std::string>& friendlyName,
                                  const boost::optional<std::string>& type,
                                  const boost::optional<std::string>& model,
                                  const std::set<std::string>& containsKeywordWithCapacityName,
                                  const boost::optional<shared::plugin::yPluginApi::EKeywordAccessMode>& containsKeywordWithAccessMode,
                                  const std::set<shared::plugin::yPluginApi::EKeywordDataType>& containsKeywordWithDataType,
                                  const boost::optional<shared::plugin::yPluginApi::EHistoryDepth>& containsKeywordWithHistoryDepth,
                                  bool blacklistedIncluded,
                                  const boost::optional<int>& page,
                                  const boost::optional<int>& pageSize,
                                  std::function<void(std::vector<boost::shared_ptr<entities::CDevice>>, int)> onDone) const
         {
            auto pagesCount = 0;
            if (page && pageSize)
            {
               const auto countQuery = m_databaseRequester->newQuery();

               countQuery->SelectCount().
                           From(CDeviceTable::getTableName()).
                           WhereTrue();

               addDevicesQueryFilters(*countQuery,
                                      deviceId,
                                      pluginInstanceId,
                                      friendlyName,
                                      type,
                                      model,
                                      containsKeywordWithCapacityName,
                                      containsKeywordWithAccessMode,
                                      containsKeywordWithDataType,
                                      containsKeywordWithHistoryDepth,
                                      blacklistedIncluded);

               pagesCount = m_databaseRequester->queryCount(*countQuery) / *pageSize + 1;
            }

            const auto query = m_databaseRequester->newQuery();

            query->Select().
                   From(CDeviceTable::getTableName()).
                   WhereTrue();

            addDevicesQueryFilters(*query,
                                   deviceId,
                                   pluginInstanceId,
                                   friendlyName,
                                   type,
                                   model,
                                   containsKeywordWithCapacityName,
                                   containsKeywordWithAccessMode,
                                   containsKeywordWithDataType,
                                   containsKeywordWithHistoryDepth,
                                   blacklistedIncluded);

            if (page && pageSize)
               query->Limit(*pageSize, *page * *pageSize);

            adapters::CDeviceAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, *query);

            const auto devices = adapter.getResults();

            onDone(devices,
                   pagesCount);
         }

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getCompatibleForMergeDevice(int refDevice) const
			{
				// A device is compatible for merge with an other when pluginId and type are the same

				const auto referenceDevice = getDevice(refDevice);

            const auto qSelect = m_databaseRequester->newQuery();
				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0).
					And(CDeviceTable::getIdColumnName(), CQUERY_OP_NOT_EQUAL, referenceDevice->Id()).
					And(CDeviceTable::getTypeColumnName(), CQUERY_OP_EQUAL, referenceDevice->Type()).
					And(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, referenceDevice->PluginId());

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

         boost::shared_ptr<entities::CDevice> CDevice::createDevice(int pluginId,
                                                                    const std::string& name,
                                                                    const std::string& friendlyName,
                                                                    const std::string& type,
                                                                    const std::string& model,
																	   boost::shared_ptr<shared::CDataContainer> details)
			{
				if (deviceExists(pluginId, name))
					throw shared::exception::CEmptyResult("The device already exists, cannot create it a new time");

				//device not found, creation is enabled

				//get a good name
				auto realFriendlyName = friendlyName;
				if (realFriendlyName.empty())
					realFriendlyName = name;

				//insert in db
            const auto qInsert = m_databaseRequester->newQuery();
				qInsert->InsertInto(CDeviceTable::getTableName(),
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
						   details->serialize());
				if (m_databaseRequester->queryStatement(*qInsert) <= 0)
					throw shared::exception::CEmptyResult("Fail to insert new device");

				//device is created, just find it in table and return entity
				auto deviceCreated = getDeviceInPlugin(pluginId, name);
				if (!deviceCreated)
					throw shared::exception::CEmptyResult("Fail to retrieve created device");

				return deviceCreated;
			}

         boost::shared_ptr<entities::CDevice> CDevice::createDevice(boost::shared_ptr<entities::CDevice> device)
         {
            if (deviceExists(device->PluginId(), device->Name()))
               throw shared::exception::CEmptyResult("Fail to create device (device already exists)");

            const auto qInsert = m_databaseRequester->newQuery();
            qInsert->InsertInto(CDeviceTable::getTableName(),
                                CDeviceTable::getPluginIdColumnName(),
                                CDeviceTable::getNameColumnName(),
                                CDeviceTable::getFriendlyNameColumnName(),
                                CDeviceTable::getTypeColumnName(),
                                CDeviceTable::getModelColumnName(),
                                CDeviceTable::getDetailsColumnName(),
                                CDeviceTable::getConfigurationColumnName(),
                                CDeviceTable::getBlacklistColumnName()).
                     Values(device->PluginId(),
                            device->Name(),
                            device->FriendlyName().empty() ? device->Name() : device->FriendlyName(),
                            device->Type(),
                            device->Model(),
                            device->Details()->serialize(),
                            device->Configuration(),
                            device->Blacklist());
            if (m_databaseRequester->queryStatement(*qInsert) <= 0)
               throw shared::exception::CEmptyResult("Error inserting new device");

            //device is created, just find it in table and return ID
            auto deviceCreated = getDeviceInPlugin(device->PluginId(),
                                                   device->Name());
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
				if (newFriendlyName.empty())
					return;

				//insert in db
            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getFriendlyNameColumnName(), newFriendlyName).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device friendlyName");
			}

			void CDevice::rename(int deviceId, const std::string& newName)
			{
				if (!deviceExists(deviceId))
					throw shared::exception::CEmptyResult("The device does not exists");

				if (newName.empty())
					throw shared::exception::CEmptyResult("Invalid name provided");

            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getNameColumnName(), newName).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device name");
			}

			void CDevice::updateDeviceConfiguration(int deviceId, boost::shared_ptr<shared::CDataContainer> configuration)
			{
				if (!deviceExists(deviceId))
					throw shared::exception::CEmptyResult("The device does not exists");

            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getConfigurationColumnName(), configuration->serialize()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device configuration");
			}

			void CDevice::updateDeviceDetails(int deviceId, boost::shared_ptr<shared::CDataContainer> details)
			{
				if (!deviceExists(deviceId))
					throw shared::exception::CEmptyResult("The device does not exists");

            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getDetailsColumnName(), details->serialize()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device details");
			}

			void CDevice::updateDeviceModel(int deviceId, const std::string& model)
			{
				if (!deviceExists(deviceId))
					throw shared::exception::CEmptyResult("The device does not exists");

            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getModelColumnName(), model).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device model");
			}

			void CDevice::updateDeviceType(int deviceId, const std::string& type)
			{
				if (!deviceExists(deviceId))
					throw shared::exception::CEmptyResult("The device does not exists");

            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getTypeColumnName(), type).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device type");
			}

			void CDevice::updateDeviceBlacklistState(int deviceId, bool blacklist)
			{
				if (!deviceExists(deviceId))
					throw shared::exception::CEmptyResult("The device does not exists");

            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getBlacklistColumnName(), blacklist).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device blacklist");
			}

			void CDevice::updateDeviceName(int deviceId,
										   const std::string& newName)
			{
				if (!deviceExists(deviceId))
					throw shared::exception::CEmptyResult("The device does not exists");

            const auto qUpdate = m_databaseRequester->newQuery();
				qUpdate->Update(CDeviceTable::getTableName()).
					Set(CDeviceTable::getNameColumnName(), newName).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);

				if (m_databaseRequester->queryStatement(*qUpdate) <= 0)
					throw shared::exception::CEmptyResult("Fail to update device name");
			}

         void CDevice::updateDevice(const entities::CDevice& device) const
         {
            if (!device.Id.isDefined())
               throw CDatabaseException("Need an id to update");

            const auto query = m_databaseRequester->newQuery();
            query->Update(CDeviceTable::getTableName());

            if (device.PluginId.isDefined())
               query->MultiSet(CDeviceTable::getPluginIdColumnName(), device.PluginId());
            if (device.Name.isDefined())
               query->MultiSet(CDeviceTable::getNameColumnName(), device.Name());
            if (device.FriendlyName.isDefined())
               query->MultiSet(CDeviceTable::getFriendlyNameColumnName(), device.FriendlyName());
            if (device.Model.isDefined())
               query->MultiSet(CDeviceTable::getModelColumnName(), device.Model());
            if (device.Details.isDefined())
               query->MultiSet(CDeviceTable::getDetailsColumnName(), device.Details());
            if (device.Configuration.isDefined())
               query->MultiSet(CDeviceTable::getConfigurationColumnName(), device.Configuration());
            if (device.Type.isDefined())
               query->MultiSet(CDeviceTable::getTypeColumnName(), device.Type());
            if (device.Blacklist.isDefined())
               query->MultiSet(CDeviceTable::getBlacklistColumnName(), device.Blacklist());

            query->Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, device.Id());

            if (m_databaseRequester->queryStatement(*query) <= 0)
               throw shared::exception::CEmptyResult("Fail to update device");
         }

			bool CDevice::isDeviceBlacklisted(int deviceId) const
			{
				const auto& device = getDevice(deviceId, true);
				return device->Blacklist();
			}

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevices(bool blacklistedIncluded) const
			{
            const auto qSelect = m_databaseRequester->newQuery();

				if (blacklistedIncluded)
					qSelect->Select().
					From(CDeviceTable::getTableName());
				else
					qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevices(const std::set<int>& deviceIds) const
			{
            const auto qSelect = m_databaseRequester->newQuery();

				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_IN, deviceIds);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

			std::vector<std::string> CDevice::getDevicesNames(int pluginId,
															  bool blacklistedIncluded) const
			{
            const auto qSelect = m_databaseRequester->newQuery();

				if (blacklistedIncluded)
					qSelect->Select(CDeviceTable::getNameColumnName()).
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);
				else
					qSelect->Select(CDeviceTable::getNameColumnName()).
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CSingleValueAdapter<std::string> adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevices(int pluginId,
																				  bool blacklistedIncluded) const
			{
            const auto qSelect = m_databaseRequester->newQuery();

				if (blacklistedIncluded)
					qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);
				else
					qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId).
					And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

			std::vector<boost::shared_ptr<entities::CDevice>> CDevice::getDevices(const std::vector<std::string>& names,
																				  bool blacklistedIncluded = false) const
			{
            const auto qSelect = m_databaseRequester->newQuery();

				qSelect->Select().
					From(CDeviceTable::getTableName()).
					Where(CDeviceTable::getNameColumnName(), CQUERY_OP_IN, names);

				if (!blacklistedIncluded)
					qSelect->And(CDeviceTable::getBlacklistColumnName(), CQUERY_OP_EQUAL, 0);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
				return adapter.getResults();
			}

			void CDevice::removeDevice(int deviceId)
			{
            const auto qDelete = m_databaseRequester->newQuery();
				qDelete->DeleteFrom(CDeviceTable::getTableName()).
					Where(CDeviceTable::getIdColumnName(), CQUERY_OP_EQUAL, deviceId);
				if (m_databaseRequester->queryStatement(*qDelete) <= 0)
					throw shared::exception::CEmptyResult("No lines affected");

				qDelete->Clear().DeleteFrom(CKeywordTable::getTableName()).
					Where(CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
				m_databaseRequester->queryStatement(*qDelete);
			}

			void CDevice::removeDevice(int pluginId, const std::string& deviceName)
			{
				removeDevice(getDeviceInPlugin(pluginId, deviceName)->Id());
			}

			void CDevice::removeAllDeviceForPlugin(int pluginId)
			{
            const auto qSelect = m_databaseRequester->newQuery();
				qSelect->Select().
					From(CDeviceTable::getTableName()).Where(CDeviceTable::getPluginIdColumnName(), CQUERY_OP_EQUAL, pluginId);

				adapters::CDeviceAdapter adapter;
				m_databaseRequester->queryEntities(&adapter, *qSelect);
            const auto devicesToDelete = adapter.getResults();

				for (const auto& i : devicesToDelete)
					removeDevice(i->Id());
			}
		} //namespace requesters
	} //namespace common
} //namespace database 
