#include "UrlManager.h"
#include "shared/http/HttpMethods.h"
#include "shared/encryption/Base64.h"
#include "shared/http/StandardSession.h"
#include <boost/make_shared.hpp>

const std::string CUrlManager::DevicePath("/api/v2/device");
const std::string CUrlManager::WifiPath("/api/v2/device/wifi");
const std::string CUrlManager::BluetoothPath("/api/v2/device/bluetooth");
const std::string CUrlManager::AudioPath("/api/v2/device/audio");

CUrlManager::CUrlManager(const CLametricConfiguration& lametricConfiguration)
   : m_lametricConfiguration(lametricConfiguration)
{
}

shared::CDataContainer CUrlManager::getState(ERequestType requestType)
{
   auto protocolType = "http://";
   auto port = m_lametricConfiguration.getPort();
   auto ipAddress = m_lametricConfiguration.getIPAddress();
   auto apiKey = m_lametricConfiguration.getAPIKey();

   std::string username = "dev";
   std::string AuthorizationType = "Basic ";
   std::string requestPath;

   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   switch (requestType)
   {
   case kRequestDevice:
      requestPath = DevicePath;
      break;
   case kRequestWifi:
      requestPath = WifiPath;
      break;
   case kRequestBluetooth:
      requestPath = BluetoothPath;
      break;
   case kRequestAudio:
      requestPath = AudioPath;
      break;
   default:
      requestPath = "";;
   }

   auto url = protocolType + ipAddress + ":" + port + requestPath;

   std::string authorizationHeader = username + ":" + apiKey;

   Poco::URI uri(url);

   headerParameters.set("Authorization",
                        AuthorizationType + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");

   const auto session = boost::make_shared<shared::CStandardSession>(url);

   shared::CHttpMethods::sendGetRequest(session,
                                        headerParameters,
                                        uri,
                                        [&](shared::CDataContainer& data)
                                        {
                                           response = data;
                                        });
   return response;
}
