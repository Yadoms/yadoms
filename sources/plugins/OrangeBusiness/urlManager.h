#pragma once

#include <shared/DataContainer.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/Socket.h>
#include <shared/http/HttpMethods.h>

//--------------------------------------------------------------
/// \brief	All url handling
//--------------------------------------------------------------
class urlManager
{
public:

   urlManager();

   //--------------------------------------------------------------
   /// \brief	    read files Status from the WES
   /// \param[in]  socket              the IP adress with the socket where to send the frame
   /// \param[in]  credentials         credentials (user, password) to access the WES
   /// \param[in]  file                file we have to access
   /// \return     the preprocess answer of the request
   //--------------------------------------------------------------
   shared::CDataContainer getAllregisteredEquipments(const std::string &apikey,
                                                     const boost::posix_time::time_duration& timeout = shared::httpRequestDefaultTimeout);

private:

   //--------------------------------------------------------------
   /// \brief	    The url link to access properly the web site
   //--------------------------------------------------------------
   std::stringstream m_url;
   std::stringstream m_baseUrl;
};