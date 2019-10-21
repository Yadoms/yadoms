#include "UrlManager.h"
#include "shared/http/HttpMethods.h"
#include "shared/encryption/Base64.h"
#include "shared/http/StandardSession.h"
#include <boost/make_shared.hpp>


shared::CDataContainer CUrlManager::getDeviceState(const std::string& ipAddress, const std::string& port,
                                                   const std::string& apikey)
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + ipAddress + ":" + port +
      "/api/v2/device";

   std::string authorizationHeader = "dev:" + apikey;

   Poco::URI uri(uriStr);

   headerParameters.set("Authorization",
                        "Basic " + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");

   const auto session = boost::make_shared<shared::CStandardSession>(uriStr);

   shared::CHttpMethods::sendGetRequest(session,
                                        headerParameters,
                                        uri,
                                        [&](shared::CDataContainer& data)
                                        {
                                           response = data;
                                        });
   return response;
}

shared::CDataContainer CUrlManager::getWifiState(const std::string& ipAddress, const std::string& port,
                                                 const std::string& apikey)
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + ipAddress + ":" + port +
      "/api/v2/device/wifi";

   std::string authorizationHeader = "dev:" + apikey;

   Poco::URI uri(uriStr);

   headerParameters.set("Authorization",
                        "Basic " + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");

   const auto session = boost::make_shared<shared::CStandardSession>(uriStr);

   shared::CHttpMethods::sendGetRequest(session,
                                        headerParameters,
                                        uri,
                                        [&](shared::CDataContainer& data)
                                        {
                                           response = data;
                                        });
   return response;
}


shared::CDataContainer CUrlManager::getBluetoothState(const std::string& ipAddress, const std::string& port,
                                                      const std::string& apikey)
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + ipAddress + ":" + port +
      "/api/v2/device/bluetooth";

   std::string authorizationHeader = "dev:" + apikey;

   Poco::URI uri(uriStr);

   headerParameters.set("Authorization",
                        "Basic " + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");

   const auto session = boost::make_shared<shared::CStandardSession>(uriStr);

   shared::CHttpMethods::sendGetRequest(session,
                                        headerParameters,
                                        uri,
                                        [&](shared::CDataContainer& data)
                                        {
                                           response = data;
                                        });
   return response;
}


shared::CDataContainer CUrlManager::getAudioState(const std::string& ipAddress, const std::string& port,
                                                  const std::string& apikey)
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + ipAddress + ":" + port +
      "/api/v2/device/audio";

   std::string authorizationHeader = "dev:" + apikey;

   Poco::URI uri(uriStr);

   headerParameters.set("Authorization",
                        "Basic " + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");

   const auto session = boost::make_shared<shared::CStandardSession>(uriStr);

   shared::CHttpMethods::sendGetRequest(session,
                                        headerParameters,
                                        uri,
                                        [&](shared::CDataContainer& data)
                                        {
                                           response = data;
                                        });
   return response;
}
