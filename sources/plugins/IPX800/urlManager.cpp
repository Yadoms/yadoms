#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"

shared::CDataContainer urlManager::sendCommand(Poco::Net::SocketAddress socket, shared::CDataContainer parameters)
{
   std::stringstream url;

   // create the URL
   url << "http://" << socket.toString() << "/api/xdevices.json";

   return http::CHttpMethods::SendGetRequest(url.str(), parameters);
}