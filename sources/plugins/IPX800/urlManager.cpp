#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"

shared::CDataContainerSharedPtr urlManager::sendCommand(Poco::Net::SocketAddress socket, shared::CDataContainerSharedPtr parameters)
{
   std::stringstream url;

   // create the URL
   url << "http://" << socket.toString() << "/api/xdevices.json";

   return http::CHttpMethods::SendGetRequest(url.str(), parameters);
}