#pragma once
#include "database/entities/Entities.h"
#include <shared/plugin/yPluginApi/historization/DeviceState.h>

namespace dataAccessLayer
{
   class IDeviceManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Check if device exists
      /// \param [in] deviceId            The device Id
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool deviceExists(int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Check if device exists
      /// \param [in] pluginId            The plugin Id
      /// \param [in] deviceName          The device name (plugin internal name)
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool deviceExists(int pluginId,
                                const std::string& deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get device information
      /// \param [in] deviceId            Device Id
      /// \throw                          shared::exception::CEmptyResult if deviceId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CDevice> getDevice(int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief                          List all devices of a plugin
      /// \param [in] pluginId            plugin Id
      /// \param [in] blacklistedIncluded True to list blacklisted devices too
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice>> getDevices(int pluginId,
                                                                                     bool blacklistedIncluded) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \param [in] includeBlacklistDevice True to list blacklisted devices too
      /// \return                         The device found
      /// \throw                          shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CDevice> getDeviceInPlugin(int pluginId,
                                                                               const std::string& name,
                                                                               bool includeBlacklistDevice) const = 0;


      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity
      /// \param [in] capacityName        The capacity name
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice>> getDeviceWithCapacity(
         const std::string& capacityName,
         const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity type
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \param [in] capacityType        The capacity capacityType
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice>> getDeviceWithCapacityType(
         const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode,
         const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the device list from device name
      /// \param [in] expectedNames       The expected name
      /// \param [in] expectedNames       The history depth criteria
      /// \param [in] blacklisted         The blacklisted criteria
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice>> getDevicesMatchingCriteria(
         const std::vector<std::string>& expectedNames,
         bool blacklisted) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Create a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \param [in] friendlyName        The user friendly device name
      /// \param [in] type                The device type (ex : "osCN185")
      /// \param [in] model               The device model or description (ex : "Oregon Scientific CN185")
      /// \param [in] details             A free string managed by plugin
      /// \return                         The device created (null if creation failed)
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CDevice> createDevice(int pluginId,
                                                                          const std::string& name,
                                                                          const std::string& friendlyName,
                                                                          const std::string& type,
                                                                          const std::string& model,
                                                                          const shared::CDataContainer& details) = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices
      /// \return          List of registered devices
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice>> getDevices() const = 0;

      //--------------------------------------------------------------
      /// \brief           Get all devices of a plugin instance
      /// \param [in] pluginId            The pluginId
      /// \return          All devices of specified plugin instance
      //--------------------------------------------------------------
      virtual std::vector<std::string> getDevicesForPluginInstance(int pluginId) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Update a device friendly name
      /// \param [in] deviceId            The device id
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceFriendlyName(int deviceId,
                                            const std::string& newFriendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device configuration
      /// \param [in] deviceId            The device id
      /// \param [in] configuration       The device configuration
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceConfiguration(int deviceId,
                                             const shared::CDataContainer& configuration) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device details
      /// \param [in] deviceId            The device id
      /// \param [in] details             The device details
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceDetails(int deviceId,
                                       const shared::CDataContainer& details) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device model
      /// \param [in] deviceId            The device id
      /// \param [in] model               The device model
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceModel(int deviceId, const std::string& model) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device type
      /// \param [in] deviceId            The device id
      /// \param [in] type               The device type
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceType(int deviceId, const std::string& type) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device blacklist state
      /// \param [in] deviceId            The device id
      /// \param [in] blacklist           The device blacklist state (propagated to all attached keywords)
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceBlacklistState(int deviceId,
                                              bool blacklist) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device state (create keyword if needed)
      /// \param [in] deviceId            The device id
      /// \param [in] customMessageId     The ID of the message
      /// \param [in] state               The device state
      /// \param [in] data                The device state message data
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceState(int deviceId,
                                     const shared::plugin::yPluginApi::historization::EDeviceState& state,
                                     const std::string& customMessageId,
                                     const shared::CDataContainer& data) const = 0;

      //--------------------------------------------------------------
      /// \brief           Remove device 
      /// \param [in] deviceId   Device  Id
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeDevice(int deviceId) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove device 
      /// \param [in] pluginId     The pluginId
      /// \param [in] deviceName   Device  name
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeDevice(int pluginId,
                                const std::string& deviceName) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove all device for one plugin
      /// \param [in]      pluginId   plugin  Id
      //--------------------------------------------------------------
      virtual void removeAllDeviceForPlugin(int pluginId) = 0;

      //--------------------------------------------------------------
      /// \brief                 Cleanup a device (erase all related acquisitions)
      /// \param [in] deviceId   The device ID to cleanup
      //--------------------------------------------------------------
      virtual void cleanupDevice(int deviceId) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IDeviceManager() = default;
   };
} //namespace dataAccessLayer 
