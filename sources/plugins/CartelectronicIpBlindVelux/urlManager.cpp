#include "stdafx.h"
#include "urlManager.h"
#include <shared/Log.h>
#include <shared/http/HttpMethods.h>

boost::posix_time::time_duration urlManager::httpRequestCreationTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(8)));
boost::posix_time::time_duration urlManager::httpRequestVRTIPTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(25)));

std::string urlManager::getRelayState(
	const Poco::Net::SocketAddress& socket)
{
	std::string out;
	shared::CDataContainer NoElements;

	shared::CHttpMethods::sendGetRequest("http://" + socket.toString() + "/ctrl.cgi",
		[&out](const Poco::Net::HTTPResponse& response, std::istream& receivedStream) {
		    out = std::string(
			std::istreambuf_iterator<char>(receivedStream), {});
	    },
		NoElements,
		NoElements,
		shared::CHttpMethods::ESessionType::kStandard,
		httpRequestVRTIPTimeout);

	return out;
}

std::string urlManager::setRelayState(
	Poco::Net::SocketAddress socket,
	const shared::CDataContainer& parameters)
{
   std::stringstream url;
   std::string out;
   shared::CDataContainer NoElements;

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

   if (parameters.exists("tm1") && parameters.exists("shutter1")) {
	   auto delay = parameters.get<long>("tm1");
	   if (delay != 0) {
		   url << "&tm1=" << boost::lexical_cast<std::string>(delay);
	   }
   }

   if (parameters.exists("tm2") && parameters.exists("shutter2")) {
	   auto delay = parameters.get<long>("tm2");
	   if (delay != 0) {
		   url << "&tm2=" << boost::lexical_cast<std::string>(delay);
	   }
   }

   YADOMS_LOG(information) << "URL : " << url.str();

   shared::CHttpMethods::sendGetRequest(url.str(),
	   [&out](const Poco::Net::HTTPResponse& response, std::istream& receivedStream) {
	   out = std::string(
		   std::istreambuf_iterator<char>(receivedStream), {});
       },
	   NoElements,
	   NoElements,
	   shared::CHttpMethods::ESessionType::kStandard,
	   httpRequestVRTIPTimeout);

   return out;
}