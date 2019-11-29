#include "stdafx.h"
#include "urlManager.h"

urlManager::urlManager():
   m_url("https://liveobjects.orange-business.com/api/v0"), //liveobjects
   m_baseUrl("liveobjects.orange-business.com")
{
}

shared::CDataContainer urlManager::getRegisteredEquipments(const std::string& apikey,
                                                           const int page,
                                                           const bool activated,
                                                           const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   // define some header parameters
   headerParameters.set("Host", m_baseUrl.str());
   headerParameters.set("X-API-Key", apikey);
   headerParameters.set("Accept", "application/json");

   parameters.set("page", std::to_string(page));

   if (activated)
      parameters.set("status", "ACTIVATED");

   const auto deviceUrl = m_url.str() + "/vendors/lora/devices";
   return shared::CHttpMethods::sendGetRequest(deviceUrl,
                                               headerParameters,
                                               parameters,
                                               shared::CHttpMethods::kSecured,
                                               timeout);
}

shared::CDataContainer urlManager::getDeviceInformation(const std::string& apikey,
                                                        const std::string& devEUI,
                                                        const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer headerParameters;
   const shared::CDataContainer noParameters;

   // define some header parameters
   headerParameters.set("Host", m_baseUrl.str());
   headerParameters.set("X-API-Key", apikey);
   headerParameters.set("Accept", "application/json");

   const auto deviceUrl = "https://liveobjects.orange-business.com/api/v0/vendors/lora/devices/" + devEUI;
   return shared::CHttpMethods::sendGetRequest(deviceUrl,
                                               headerParameters,
                                               noParameters,
                                               shared::CHttpMethods::kSecured,
                                               timeout);
}

shared::CDataContainer urlManager::listDeviceCommands(const std::string& apikey,
                                                      const std::string& devEUI,
                                                      const int page,
                                                      const boost::posix_time::time_duration& timeout)
{
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   // define some header parameters
   headerParameters.set("X-API-Key", apikey);
   headerParameters.set("Accept", "application/json");

   parameters.set("limit", "5");
   parameters.set("sort", "-creationTs"); // Get the newest command at the first page
   const auto deviceUrl = "https://liveobjects.orange-business.com/api/v0/data/streams/urn:lora:" + devEUI + "!uplink";
   return shared::CHttpMethods::sendGetRequest(deviceUrl,
                                               headerParameters,
                                               parameters,
                                               shared::CHttpMethods::kSecured,
                                               timeout);
}
