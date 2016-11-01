#pragma once

//#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include <Poco/Net/NetworkInterface.h>

// Shortcut to yPluginApi namespace
//namespace yApi = shared::plugin::yPluginApi;

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
   static bool sendCommand(Poco::Net::IPAddress IPAddress, shared::CDataContainer parameters);

};