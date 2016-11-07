#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"

shared::CDataContainer urlManager::sendCommand(Poco::Net::IPAddress IPAddress, shared::CDataContainer parameters)
{
   std::stringstream url;

   // create the URL
   url << "http://" << IPAddress.toString() << "/api/xdevices.json";
   std::cout << url.str();

   return http::CHttpMethods::SendGetRequest(url.str(), parameters);
}