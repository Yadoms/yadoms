#pragma once

#include <shared/DataContainer.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/Socket.h>
#include "http/HttpMethods.h"
#include "http/HttpContext.h"
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
   /// \param[in]  credentials         credentials(user, password) needed to access the WES
   /// \param[in]  parameters          informations to be send
   /// \param[in]  context             context specific for each equipment for sending information
   /// \return     the answer of the request
   //--------------------------------------------------------------
   static shared::CDataContainer getRelayState(
	   Poco::Net::SocketAddress socket,
	   const shared::CDataContainer& credentials,
	   const shared::CDataContainer& parameters,
	   http::httpContext& context);

   //--------------------------------------------------------------
   /// \brief	    set relays state to the WES
   /// \param[in]  socket              the IP adress with the socket where to send the frame
   /// \param[in]  credentials         credentials(user, password) needed to access the WES
   /// \param[in]  parameters          informations to be send
   /// \param[in]  context             context specific for each equipment for sending information
   /// \return     the answer of the request
   //--------------------------------------------------------------
   static shared::CDataContainer setRelayState(
	   Poco::Net::SocketAddress socket,
       const shared::CDataContainer& credentials,
	   const shared::CDataContainer& parameters,
	   http::httpContext& context);
};