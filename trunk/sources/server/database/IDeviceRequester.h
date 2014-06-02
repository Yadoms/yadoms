#pragma once

#include "entities/Entities.h"


namespace database { 


   class IDeviceRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief                 Get device informations
      /// \param [in] deviceId   Device Id
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CDevice> getDevice(int deviceId) = 0;

      //--------------------------------------------------------------
      /// \brief                          Get a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \return                         The device found (null if not found)
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CDevice> getDevice(const int pluginId, const std::string & name) = 0;


      //--------------------------------------------------------------
      /// \brief                          Get the device list which support a capaicty
      /// \param [in] capacityName        The capacity name
      /// \param [in] capacityAccessMode  The capacity access mode
      /// \return                         the device list which support a capaicty
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice> > getDeviceWithCapacity(const std::string & capacityName, const database::entities::ECapacityAccessMode capacityAccessMode) =0;

      //--------------------------------------------------------------
      /// \brief                          Create a device identified by (pluginId and name).
      /// \param [in] pluginId            The pluginId
      /// \param [in] name                The device name (plugin internal name)
      /// \param [in] friendlyName        The user friendly device name
      /// \return                         The device created (null if creation failed)
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CDevice> createDevice(int pluginId, const std::string & name, const std::string & friendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices
      /// \return          List of registered devices
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice> > getDevices() = 0;

     
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
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IDeviceRequester()
      {
      }
   };

 
} //namespace database 
   