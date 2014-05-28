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
      /// \brief                 Get the device last data
      /// \param [in] deviceId   Device Id
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string>  > getDeviceLastData(int deviceId) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the device data
      /// \param [in] deviceId   Device Id
      /// \param [in] keyword    The keyword
      /// \param [in] timeFrom   The start date (optionnal)
      /// \param [in] timeTo     The end date (optionnal)
      /// \return                Map of data : (date, value)
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string>  > getDeviceData(int deviceId, const std::string & keyword,  boost::posix_time::ptime timeFrom, boost::posix_time::ptime timeTo ) = 0;

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
   