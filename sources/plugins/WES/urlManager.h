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
   /// \param[in]  socket              the IP adress with the socket where to send the frame
   /// \param[in]  parameters          extra-parameters to the url
   /// \return     the answer of the request
   //--------------------------------------------------------------
   static shared::CDataContainer sendCommand(Poco::Net::SocketAddress socket, 
                                             std::string &user, 
                                             std::string &password,
                                             std::string &file);

};