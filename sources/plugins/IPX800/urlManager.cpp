#include "stdafx.h"
#include "urlManager.h"
#include <shared/http/HttpMethods.h>

shared::CDataContainer urlManager::sendCommand(Poco::Net::IPAddress IPAddress, shared::CDataContainer parameters)
{
   std::stringstream url;

   // create the URL
   url << "http://" << IPAddress.toString() << "/api/xdevices.json";
   std::cout << url.str();

   parameters.printToLog();

   shared::CDataContainer results = shared::CHttpMethods::SendGetRequest(url.str(), parameters);

   results.printToLog();

   return results;

   /*
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
   }*/
}