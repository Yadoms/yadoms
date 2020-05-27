#pragma once

#include "entities/Entities.h"


namespace database
{
   class IDeviceRequester
   {
   public:
      virtual ~IDeviceRequester() = default;

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
      /// \param [in] blacklistedIncluded Even if device is blacklisted
      /// \throw                          shared::exception::CEmptyResult if deviceId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CDevice> getDevice(int deviceId,
                                                             bool blacklistedIncluded = false) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \param [in] blacklistedIncluded Even if device is blacklisted
      /// \return                         The device found
      /// \throw                          shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CDevice> getDeviceInPlugin(int pluginId,
                                                                     const std::string& name,
                                                                     bool blacklistedIncluded = false) const =
      0;

      //--------------------------------------------------------------
      /// \brief                          Get devices identified by a friendly name.
      /// \param [in] friendlyName        The device friendly name
      /// \return                         The list of found devices
      /// \throw                          shared::exception::CEmptyResult if none found
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDevicesIdFromFriendlyName(
         const std::string& friendlyName) const = 0;


      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity
      /// \param [in] capacityName        The capacity name
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithCapacity(const std::string& capacityName,
                                                                                      const shared::plugin::yPluginApi::
                                                                                      EKeywordAccessMode&
                                                                                      capacityAccessMode) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity type
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \param [in] capacityType        The capacity capacityType
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithCapacityType(
         const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode,
         const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the device list which contains at least one keyword with matching access mode
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \return                         the device list
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithKeywordAccessMode(
         const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the device list which contains at least one keyword with matching history depth policy
      /// \param [in] historyDepth        The history depth
      /// \return                         the device list
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDeviceWithKeywordHistoryDepth(
         const shared::plugin::yPluginApi::EHistoryDepth& historyDepth) const = 0;


      //--------------------------------------------------------------
      /// \brief                          Get the compatible device list which is compatible to a given device
      /// \param [in] refDevice           The reference device ID
      /// \return                         the device list
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getCompatibleForMergeDevice(int refDevice) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Create a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \param [in] friendlyName        The user friendly device name
      /// \param [in] type                The device type
      /// \param [in] model               The device model or description (ex : "Oregon Scientific CN185")
      /// \param [in] details             The device details
      /// \return                         The device created (null if creation failed)
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CDevice> createDevice(int pluginId,
                                                                const std::string& name,
                                                                const std::string& friendlyName,
                                                                const std::string& type,
                                                                const std::string& model,
                                                                boost::shared_ptr<shared::CDataContainer> details) = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices
      /// \param [in] blacklistedIncluded Return also blacklisted devices
      /// \return          List of registered devices
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDevices(bool blacklistedIncluded = false) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get devices with their properties from device ID list
      /// \param [in] deviceIds Device IDs list
      /// \return          List of registered devices
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDevices(const std::set<int>& deviceIds) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices attached to a plugin instance
      /// \param [in] pluginId            The pluginId
      /// \param [in] blacklistedIncluded Return also blacklisted devices
      /// \return          List of registered devices attached to a plugin instance
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDevices(int pluginId,
                                                                           bool blacklistedIncluded = false) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices matching a list of names
      /// \param [in] names            The expected names list
      /// \param [in] blacklistedIncluded Return also blacklisted devices
      /// \return          List of registered devices matching at least one of names
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice>> getDevices(const std::vector<std::string>& names,
                                                                           bool blacklistedIncluded = false) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices attached to a plugin instance, return only names
      /// \param [in] pluginId            The pluginId
      /// \param [in] blacklistedIncluded Return also blacklisted devices
      /// \return          List of devices names attached to a plugin instance
      //--------------------------------------------------------------
      virtual std::vector<std::string> getDevicesNames(int pluginId,
                                                       bool blacklistedIncluded = false) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Update a device friendly name
      /// \param [in] deviceId            The device id
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceFriendlyName(int deviceId,
                                            const std::string& newFriendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Rename a device (use carefully, this name is used by plugin to historize data)
      /// \param [in] deviceId            The device id
      /// \param [in] newName             The new name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void rename(int deviceId,
                          const std::string& newName) = 0;


      //--------------------------------------------------------------
      /// \brief                          Update the device configuration
      /// \param [in] deviceId            The device id
      /// \param [in] configuration       The device configuration
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceConfiguration(int deviceId,
                                             boost::shared_ptr<shared::CDataContainer> configuration) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device details
      /// \param [in] deviceId            The device id
      /// \param [in] details             The new details
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceDetails(int deviceId,
                                       boost::shared_ptr<shared::CDataContainer> details) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device model
      /// \param [in] deviceId            The device id
      /// \param [in] model               The new model
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceModel(int deviceId,
                                     const std::string& model) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device type
      /// \param [in] deviceId            The device id
      /// \param [in] type                The new type
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceType(int deviceId,
                                    const std::string& type) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the device blacklist state
      /// \param [in] deviceId            The device id
      /// \param [in] blacklist           The device blacklist state
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceBlacklistState(int deviceId,
                                              bool blacklist) = 0;

      //--------------------------------------------------------------
      /// \brief                          Check if device is blacklisted
      /// \param [in] deviceId            The device id
      /// \return                         true if blacklisted
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual bool isDeviceBlacklisted(int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief           Remove device 
      /// \param [in] deviceId   Device Id
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeDevice(int deviceId) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove device
      /// \param [in] pluginId   Plugin Id
      /// \param [in] deviceName Device to remove
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeDevice(int pluginId,
                                const std::string& deviceName) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove all device for one plugin
      /// \param [in]      pluginId   plugin  Id
      //--------------------------------------------------------------
      virtual void removeAllDeviceForPlugin(int pluginId) = 0;
   };
} //namespace database 
