#pragma once
#include "IDeviceManager.h"
#include "database/IDeviceRequester.h"

namespace dataAccessLayer {

   class CDeviceManager : public IDeviceManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Constructor
      /// \param [in] deviceRequester     The device requester
      //--------------------------------------------------------------
      explicit CDeviceManager(boost::shared_ptr<database::IDeviceRequester> deviceRequester);
   
      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CDeviceManager();
      
      //--------------------------------------------------------------
      /// \brief                          Check if device exists
      /// \param [in] deviceId            The device Id
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool deviceExists(const int deviceId) const;

      //--------------------------------------------------------------
      /// \brief                          Check if device exists
      /// \param [in] pluginId            The plugin Id
      /// \param [in] name                The device name (plugin internal name)
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool deviceExists(const int pluginId, const std::string & deviceName) const;

      //--------------------------------------------------------------
      /// \brief                          Get device informations
      /// \param [in] deviceId            Device Id
      /// \throw                          shared::exception::CEmptyResult if deviceId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CDevice> getDevice(int deviceId) const;

      //--------------------------------------------------------------
      /// \brief                          Get a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \return                         The device found
      /// \throw                          shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CDevice> getDevice(const int pluginId, const std::string & name) const;


      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity
      /// \param [in] capacityName        The capacity name
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice> > getDeviceWithCapacity(const std::string & capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode) const;

      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capacity type
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \param [in] capacityType        The capacity capacityType
      /// \return                         the device list which support a capacity
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice> > getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode & capacityAccessMode, const shared::plugin::yPluginApi::EKeywordDataType & capacityType) const;

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
      virtual boost::shared_ptr<database::entities::CDevice> createDevice(int pluginId, const std::string & name, const std::string & friendlyName, const std::string & model, const shared::CDataContainer & details);

      //--------------------------------------------------------------
      /// \brief           List all devices
      /// \return          List of registered devices
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice> > getDevices() const;

     
      //--------------------------------------------------------------
      /// \brief                          Update a device friendly name
      /// \param [in] deviceId            The device id
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateDeviceFriendlyName(int deviceId, const std::string & newFriendlyName);

      //--------------------------------------------------------------
      /// \brief                          Update the device configuration
      /// \param [in] deviceId            The device id
      /// \param [in] configuration       The device configuration
      /// \return                         The device updated (null if update failed)
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CDevice> updateDeviceConfiguration(int deviceId, const shared::CDataContainer & configuration);

      //--------------------------------------------------------------
      /// \brief           Remove device 
      /// \param [in] deviceId   Device  Id
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeDevice(int deviceId);

      //--------------------------------------------------------------
      /// \brief           Remove all device for one plugin
      /// \param [in]      pluginId   plugin  Id
      //--------------------------------------------------------------
      virtual void removeAllDeviceForPlugin(int pluginId);
   
   protected:
      //--------------------------------------------------------------
      /// \brief           The real data requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IDeviceRequester> m_deviceRequester;
   };
 
} //namespace dataAccessLayer 
