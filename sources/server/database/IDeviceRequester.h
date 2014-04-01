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
      /// \brief                          Get or create a device
      /// \param [in] address             The device address  (i.e. : temp2 0x1234)
      /// \param [in] protocol            The device commuinication protocol (i.e. : ac.basic)
      /// \param [in] hardwareIdentifier   The hardware identifier (i.e. : rfxcomlan-0112312)
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CDevice> getDevice(const std::string & address, const std::string & protocol, const std::string & hardwareIdentifier) = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices
      /// \return          List of registered devices
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CDevice> > getDevices() = 0;

      //--------------------------------------------------------------
      /// \brief           List all devices which supports the keyword
      /// \param [in]      keyword   The keyword to check
      /// \return          List of devices which supports the keyword
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CDevice> > getDevicesMatchingKeyword(const std::string & keyword) = 0;

      //--------------------------------------------------------------
      /// \brief                 Get the device last data
      /// \param [in] deviceId   Device Id
      /// \throw                 CInvalidParameter if deviceId is unknown
      //--------------------------------------------------------------
      virtual std::vector< boost::tuple<boost::posix_time::ptime, std::string, std::string>  > getDeviceLastData(int deviceId) = 0;

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
   