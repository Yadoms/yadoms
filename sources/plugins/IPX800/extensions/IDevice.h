#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>

//--------------------------------------------------------------
/// \brief	Interface of device
//--------------------------------------------------------------
namespace extensions
{
   class IDevice
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
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IDevice()
      {}

   };
}// namespace extensions