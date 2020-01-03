#pragma once

#include <shared/DataContainer.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/Socket.h>
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	All url handling
//--------------------------------------------------------------
class urlManager
{
public:

   static boost::posix_time::time_duration httpRequestCreationTimeout;
   static boost::posix_time::time_duration httpRequestVRTIPTimeout;

   //--------------------------------------------------------------
   /// \brief	    set relays state to the WES
   /// \param[in]  socket              the IP adress with the socket where to send the frame
   /// \return     the answer of the request
   //--------------------------------------------------------------
   static std::string getRelayState(
	   const Poco::Net::SocketAddress& socket);

   //--------------------------------------------------------------
   /// \brief	    set relays state to the WES
   /// \param[in]  socket              the IP adress with the socket where to send the frame
   /// \param[in]  parameters          informations to be send
   /// \return     the answer of the request
   //--------------------------------------------------------------
   static std::string setRelayState(
	   Poco::Net::SocketAddress socket,
	   const shared::CDataContainer& parameters);
};