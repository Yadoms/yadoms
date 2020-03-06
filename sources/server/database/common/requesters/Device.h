#pragma once

#include "database/IDeviceRequester.h"
#include "database/IDatabaseRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Device requester for SQLite database
         //--------------------------------------------------------------
         class CDevice : public IDeviceRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester the database requester
            //--------------------------------------------------------------
            explicit CDevice(boost::shared_ptr<IDatabaseRequester> databaseRequester);

            virtual ~CDevice() = default;

            // IDeviceRequester implementation
            bool deviceExists(int deviceId) const override;
            bool deviceExists(int pluginId,
                              const std::string& deviceName) const override;
            boost::shared_ptr<entities::CDevice> getDevice(int deviceId,
                                                           bool blacklistedIncluded = false) const override;
            boost::shared_ptr<entities::CDevice> getDeviceInPlugin(int pluginId,
                                                                   const std::string& name,
                                                                   bool blacklistedIncluded = false) const override;
            boost::shared_ptr<entities::CDevice> createDevice(int pluginId,
                                                              const std::string& name,
                                                              const std::string& friendlyName,
                                                              const std::string& type,
                                                              const std::string& model,
                                                              shared::CDataContainerSharedPtr details) override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDevices(bool blacklistedIncluded = false) const
            override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDevices(const std::set<int>& deviceIds) const override;
            std::vector<std::string> getDevicesNames(int pluginId,
                                                     bool blacklistedIncluded = false) const override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDevices(int pluginId,
                                                                         bool blacklistedIncluded = false) const
            override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDevices(const std::vector<std::string>& names,
                                                                         bool blacklistedIncluded) const override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDevicesIdFromFriendlyName(const std::string& friendlyName) const override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithCapacity(const std::string& capacityName,
                                                                                    const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode)
            const override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithCapacityType(
               const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode,
               const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithKeywordAccessMode(
               const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const override;
            std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithKeywordHistoryDepth(
               const shared::plugin::yPluginApi::EHistoryDepth& historyDepth) const override;
            std::vector<boost::shared_ptr<entities::CDevice>> getCompatibleForMergeDevice(int refDevice) const override;
            void updateDeviceFriendlyName(int deviceId,
                                          const std::string& newFriendlyName) override;
            void rename(int deviceId, const std::string& newName) override;
            void updateDeviceConfiguration(int deviceId,
                                           shared::CDataContainerSharedPtr configuration) override;
            void updateDeviceDetails(int deviceId,
                                     shared::CDataContainerSharedPtr details) override;
            void updateDeviceModel(int deviceId,
                                   const std::string& model) override;
            void updateDeviceType(int deviceId,
                                  const std::string& type) override;
            void updateDeviceBlacklistState(int deviceId,
                                            bool blacklist) override;
            bool isDeviceBlacklisted(int deviceId) const override;
            void removeDevice(int deviceId) override;
            void removeDevice(int pluginId,
                              const std::string& deviceName) override;
            void removeAllDeviceForPlugin(int pluginId) override;
            // [END] IDeviceRequester implementation

         private:
            boost::shared_ptr<IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 
