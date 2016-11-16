#pragma once

#include <shared/DataContainer.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/Socket.h>

//--------------------------------------------------------------
/// \brief	All url handling
//--------------------------------------------------------------
class urlManager
{
public:

   //--------------------------------------------------------------
   /// \brief	    SendUrlRequest
   /// \param[in]  url                 the url to send the request
   /// \return     the answer of the request
   //--------------------------------------------------------------
   static shared::CDataContainer sendCommand(/*Poco::Net::IPAddress IPAddress,*/ Poco::Net::SocketAddress socket, shared::CDataContainer parameters);

};