#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>

//--------------------------------------------------------------
/// \brief	Interface of device
//--------------------------------------------------------------
namespace devices
{
   class IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IDevice()
      {}

   };
}// namespace devices