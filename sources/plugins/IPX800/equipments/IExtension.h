#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Interface of device
//--------------------------------------------------------------
namespace extensions
{
   class IExtension
   {
   public:

      //--------------------------------------------------------------
      /// \brief      getDeviceName
      /// \return  the device name
      //--------------------------------------------------------------
      virtual std::string getDeviceName() const = 0;

      //--------------------------------------------------------------
      /// \brief      getSlot 
      /// \return  the slot position
      //--------------------------------------------------------------
      virtual int getSlot() const = 0;

      //--------------------------------------------------------------
      /// \brief      updateFromDevice
      ///\param[in] values     New Values to enter
      //--------------------------------------------------------------
      virtual void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer& values) const = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IExtension()
      {}

   };
}// namespace extensions