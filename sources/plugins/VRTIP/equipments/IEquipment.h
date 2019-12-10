#pragma once
#include <shared/DataContainer.h>
#include "specificHistorizers/deviceStatus.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Interface of device
//--------------------------------------------------------------
namespace equipments
{
   class IEquipment
   {
   public:

      //--------------------------------------------------------------
      /// \brief      getDeviceName
      /// \return  the device name
      //--------------------------------------------------------------
      virtual std::string getDeviceName() const = 0;

      //--------------------------------------------------------------
      /// \brief      getDeviceType
      /// \return  the device type
      //--------------------------------------------------------------
      virtual std::string getDeviceType() const = 0;

      //--------------------------------------------------------------
      /// \brief      updateConfiguration    build the message to send to the WES
      ///\param[in] api                       Yadoms API
      ///\param[in] newConfiguration          New device configuration
      ///\param[in] refreshEvent              Event to fire a device refresh after updating configuration
      //--------------------------------------------------------------
      virtual void updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                       const shared::CDataContainer& newConfiguration,
                                       const int refreshEvent) = 0;

      //--------------------------------------------------------------
      /// \brief      sendCommand             send a message to the WES
      ///\param[in] api                       Yadoms API
      ///\param[in] keyword                   keyword to change
      ///\param[in] command                   the command received from the Yadoms web client
      //--------------------------------------------------------------
      virtual void sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& keyword,
                               boost::shared_ptr<const yApi::IDeviceCommand> command) = 0;

      //--------------------------------------------------------------
      /// \brief                              get the device status (Ok, timeOut, Error, ...)
      ///\return                              the status
      //--------------------------------------------------------------
      virtual specificHistorizers::EVRTIPdeviceStatus getStatus() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IEquipment()
      {}

   };
}// namespace equipments