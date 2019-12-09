#pragma once

#include <shared/DataContainer.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/Socket.h>
#include "http/HttpMethods.h"
#include "http/HttpContext.h"

//--------------------------------------------------------------
/// \brief	All url handling
//--------------------------------------------------------------
class urlManager
{
public:

   static boost::posix_time::time_duration httpRequestCreationTimeout;
   static boost::posix_time::time_duration httpRequestWESTimeout;

   //--------------------------------------------------------------
   /// \brief	    read files Status from the WES
   /// \param[in]  socket              the IP adress with the socket where to send the frame
   /// \param[in]  credentials         credentials (user, password) to access the WES
   /// \param[in]  file                file we have to access
   /// \param[in]  context             context specific for each equipment for sending information
   /// \param[in]  timeout             time out of the request
   /// \return     the preprocess answer of the request
   //--------------------------------------------------------------
   static shared::CDataContainer readFileState(
	   Poco::Net::SocketAddress socket,
       const shared::CDataContainer& credentials,
       const std::string &file,
	   http::httpContext& context,
       const boost::posix_time::time_duration& timeout = http::CHttpMethods::httpRequestDefaultTimeout);

   //--------------------------------------------------------------
   /// \brief	    set relays state to the WES
   /// \param[in]  socket              the IP adress with the socket where to send the frame
   /// \param[in]  credentials         credentials(user, password) needed to access the WES
   /// \param[in]  parameters          extra-parameters to the url
   /// \param[in]  context             context specific for each equipment for sending information
   /// \return     the answer of the request
   //--------------------------------------------------------------
   static shared::CDataContainer setRelayState(
	   Poco::Net::SocketAddress socket,
       const shared::CDataContainer& credentials,
       const shared::CDataContainer& parameters,
	   http::httpContext& context);
};