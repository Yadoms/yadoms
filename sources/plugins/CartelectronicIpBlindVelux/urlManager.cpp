#include "stdafx.h"
#include "urlManager.h"
#include <shared/Log.h>

boost::posix_time::time_duration urlManager::httpRequestCreationTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(8)));
boost::posix_time::time_duration urlManager::httpRequestVRTIPTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(25)));

shared::CDataContainer urlManager::setRelayState(
	Poco::Net::SocketAddress socket,
    const shared::CDataContainer& credentials,
	const yApi::historization::ECurtainCommand& state,
	http::httpContext& context)
{
   std::stringstream url;

   // create the URL
   url << "http://" << socket.toString() << "/ctrl.cgi?vr1=" + boost::lexical_cast<std::string>(state.toInteger());
   YADOMS_LOG(information) << "URL : " << url.str();

   return http::CHttpMethods::SendGetRequest(
	   url.str(),
       credentials, 
	   shared::CDataContainer(),
	   context,
       httpRequestVRTIPTimeout);
}