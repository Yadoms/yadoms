#include "stdafx.h"
#include "urlManager.h"
#include <shared/Log.h>

boost::posix_time::time_duration urlManager::httpRequestCreationTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(8)));
boost::posix_time::time_duration urlManager::httpRequestVRTIPTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(25)));

shared::CDataContainer urlManager::setRelayState(
	Poco::Net::SocketAddress socket,
    const shared::CDataContainer& credentials,
	const shared::CDataContainer& parameters,
	http::httpContext& context)
{
   std::stringstream url;

   parameters.printToLog(YADOMS_LOG(information));

   // create the URL
   url << "http://" << socket.toString() << "/ctrl.cgi?";
   if (parameters.exists("shutter1")) {
	   url << "vr1=" << parameters.get<std::string>("shutter1");
   }

   if (parameters.exists("shutter2")) {
	   if (parameters.exists("shutter1")) {
		   url << "&";
	   }
	   url << "vr2=" << parameters.get<std::string>("shutter2");
   }

   YADOMS_LOG(information) << "URL : " << url.str();

   return http::CHttpMethods::SendGetRequest(
	   url.str(),
       credentials, 
	   shared::CDataContainer(),
	   context,
       httpRequestVRTIPTimeout);
}