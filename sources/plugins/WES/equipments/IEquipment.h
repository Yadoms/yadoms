#pragma once
#include <shared/DataContainer.h>
#include "../IWESConfiguration.h"
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
      /// \brief      updateFromDevice
      ///\param[in]   api                     Yadoms API
      ///\param[in]   pluginConfiguration     Configuration of the plugin
      ///\param[in]   forceHistorization      force historization for all values (initialization of the device, reset of the device, ...)
      //--------------------------------------------------------------
      virtual void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                    const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                                    bool forceHistorization) = 0;

      //--------------------------------------------------------------
      /// \brief      updateConfiguration    build the message to send to the WES
      ///\param[in] api                       Yadoms API
      ///\param[in] newConfiguration          New device configuration
      ///\param[in] refreshEvent              Event to fire a device refresh after updating configuration
      //--------------------------------------------------------------
      virtual void updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                       const shared::CDataContainerSharedPtr& newConfiguration,
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
      /// \brief      remove             remove this equipment, with all subequipment
      ///\param[in] api                       Yadoms API
      //--------------------------------------------------------------
      virtual void remove(boost::shared_ptr<yApi::IYPluginApi> api) = 0;

      //--------------------------------------------------------------
      /// \brief                              get the device status (Ok, timeOut, Error, ...)
      ///\return                              the status
      //--------------------------------------------------------------
      virtual specificHistorizers::EWESdeviceStatus getStatus() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IEquipment()
      {}

   };
}// namespace equipments