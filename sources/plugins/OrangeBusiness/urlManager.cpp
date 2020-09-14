#include "stdafx.h"
#include "urlManager.h"

urlManager::urlManager():
   m_url("https://liveobjects.orange-business.com/api/v0"), //liveobjects
   m_baseUrl("liveobjects.orange-business.com")
{
}

boost::shared_ptr<shared::CDataContainer> urlManager::getRegisteredEquipments(const std::string& apikey,
                                                                              const int page,
                                                                              const bool activated,
                                                                              int timeoutSeconds)
{
   const std::map<std::string, std::string> headerParameters = {
      {"Host", m_baseUrl.str()},
      {"X-API-Key", apikey},
      {"Accept", "application/json"}
   };

   std::map<std::string, std::string> parameters = {
      {"page", std::to_string(page)}
   };

   if (activated)
      parameters["status"] = "ACTIVATED";

   const auto deviceUrl = m_url.str() + "/vendors/lora/devices";
   return shared::http::CHttpMethods::sendJsonGetRequest(deviceUrl,
                                                         headerParameters,
                                                         parameters,
                                                         timeoutSeconds);
}

boost::shared_ptr<shared::CDataContainer> urlManager::getDeviceInformation(const std::string& apikey,
                                                                           const std::string& devEUI,
                                                                           int timeoutSeconds)
{
   const std::map<std::string, std::string> headerParameters = {
      {"Host", m_baseUrl.str()},
      {"X-API-Key", apikey},
      {"Accept", "application/json"}
   };

   const auto deviceUrl = "https://liveobjects.orange-business.com/api/v0/vendors/lora/devices/" + devEUI;
   return shared::http::CHttpMethods::sendJsonGetRequest(deviceUrl,
                                                         headerParameters,
                                                         std::map<std::string, std::string>(),
                                                         timeoutSeconds);
}

boost::shared_ptr<shared::CDataContainer> urlManager::listDeviceCommands(const std::string& apikey,
                                                                         const std::string& devEUI,
                                                                         const int page,
                                                                         int timeoutSeconds)
{
   const std::map<std::string, std::string> headerParameters = {
      {"X-API-Key", apikey},
      {"Accept", "application/json"}
   };

   const std::map<std::string, std::string> parameters = {
      {"limit", "5"},
      {"sort", "-creationTs"} // Get the newest command at the first page
   };

   const auto deviceUrl = "https://liveobjects.orange-business.com/api/v0/data/streams/urn:lora:" + devEUI + "!uplink";
   return shared::http::CHttpMethods::sendJsonGetRequest(deviceUrl,
                                                         headerParameters,
                                                         parameters,
                                                         timeoutSeconds);
}
