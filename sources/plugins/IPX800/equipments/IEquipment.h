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
      /// \return  the device type
      //--------------------------------------------------------------
      virtual void historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command) = 0;

      //--------------------------------------------------------------
      /// \brief      historizePendingCommand
      /// \return  the device type
      //--------------------------------------------------------------
      virtual void resetPendingCommand() = 0;

      //--------------------------------------------------------------
      /// \brief      updateFromDevice
      ///\param[in] values     New Values to enter
      //--------------------------------------------------------------
      virtual void updateFromDevice(const std::string& type, boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& values, bool forceHistorization) = 0;

      //--------------------------------------------------------------
      /// \brief      buildMessageToDevice
      ///\param[in] values     New Values to enter
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