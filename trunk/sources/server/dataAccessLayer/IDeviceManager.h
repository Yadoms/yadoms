#pragma once
#include "database/entities/Entities.h"

namespace dataAccessLayer {

   class IDeviceManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Check if device exists
      /// \param [in] deviceId            The device Id
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool deviceExists(const int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Check if device exists
      /// \param [in] pluginId            The plugin Id
      /// \param [in] name                The device name (plugin internal name)
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool deviceExists(const int pluginId, const std::string & deviceName) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get device informations
      /// \param [in] deviceId            Device Id
      /// \throw                          shared::exception::CEmptyResult if deviceId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CDevice> getDevice(int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \return                         The device found
      /// \throw                          shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CDevice> getDevice(const int pluginId, const std::string & name) const = 0;


      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity
      /// \param [in] capacityName        The capacity name
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<const boost::shared_ptr<const database::entities::CDevice> > getDeviceWithCapacity(const std::string & capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity type
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \param [in] capacityType        The capacity capacityType
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<const boost::shared_ptr<const database::entities::CDevice> > getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode, const shared::plugin::yPluginApi::EKeywordDataType & capacityType) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Create a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \param [in] friendlyName        The user friendly device name
      /// \param [in] model               The device model or description (ex : "Oregon Scientific CN185")
      /// \param [in] model               A free string managed by plugin
      /// \return                         The device created (null if creation failed)
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CDevice> createDevice(int pluginId, const std::string & name, const std::string & friendlyName, const std::string & model, const shared::CDataContainer & details) = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices
      /// \return          List of registered devices
      //--------------------------------------------------------------
      virtual std::vector<const boost::shared_ptr<const database::entities::CDevice> > getDevices() const = 0;

     
      //--------------------------------------------------------------
      /// \brief                          Update a device friendly name
      /// \param [in] deviceId            The device id
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceFriendlyName(int deviceId, const std::string & newFriendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove device 
      /// \param [in] deviceId   Device  Id
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeDevice(int deviceId) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove all device for one plugin
      /// \param [in]      pluginId   plugin  Id
      //--------------------------------------------------------------
      virtual void removeAllDeviceForPlugin(int pluginId) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IDeviceManager()
      {
      }
   };
 
} //namespace dataAccessLayer 
