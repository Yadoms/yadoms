#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>

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
      /// \brief      getSlot
      /// \return  the position of a device
      //--------------------------------------------------------------
      virtual int getSlot() const = 0;

      //--------------------------------------------------------------
      /// \brief      historizePendingCommand
      ///\param[in]   api         Yadoms API
      ///\param[in]   command     the pending command to historize
      //--------------------------------------------------------------
      virtual void historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command) = 0;

      //--------------------------------------------------------------
      /// \brief      resetPendingCommand  reset the pending command, if necessary (error return by the device, ...)
      /// \return  the device type
      //--------------------------------------------------------------
      virtual void resetPendingCommand() = 0;

      //--------------------------------------------------------------
      /// \brief      updateFromDevice
      ///\param[in]   api                     Yadoms API
      ///\param[in]   values                  New Values to enter
      ///\param[in]   forceHistorization      force historization for all values (initialization of the device, reset of the device, ...)
      //--------------------------------------------------------------
      virtual void updateFromDevice(const std::string& type, boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& values, bool forceHistorization) = 0;

      //--------------------------------------------------------------
      /// \brief      buildMessageToDevice    build the message to send to the IPX800
      ///\param[in]   api                     Yadoms API
      ///\param[in] parameters                parameters that have to be included in addition of elements processed into this function
      ///\param[in] command                   the command received from the Yadoms web client
      //--------------------------------------------------------------
      virtual shared::CDataContainer buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                          shared::CDataContainer& parameters,
                                                          boost::shared_ptr<const yApi::IDeviceCommand> command) = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IEquipment()
      {}

   };
}// namespace equipments