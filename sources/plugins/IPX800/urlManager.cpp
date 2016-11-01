#include "stdafx.h"
#include "urlManager.h"
#include <shared/http/HttpMethods.h>

bool urlManager::sendCommand(Poco::Net::IPAddress IPAddress, shared::CDataContainer parameters)
{
   std::stringstream url;

   // create the URL
   url << "http://" << IPAddress.toString() << "/api/xdevices.json";
   std::cout << url.str();

   shared::CDataContainer data = shared::CHttpMethods::SendGetRequest(url.str(), parameters);

   try {
      auto returnValue = data.containsValue("Success");

      std::cout << "ok" << std::endl;
      std::cout << returnValue << std::endl;

      return true;
   }
   catch (...)
   {
      std::cout << "not found" << std::endl;
      return false;
   }
}