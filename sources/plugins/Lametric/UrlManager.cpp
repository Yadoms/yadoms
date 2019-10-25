#include "UrlManager.h"
#include "shared/http/HttpMethods.h"
#include "shared/encryption/Base64.h"
#include "shared/http/StandardSession.h"

const std::string CUrlManager::DevicePath("/api/v2/device");
const std::string CUrlManager::WifiPath("/api/v2/device/wifi");
const std::string CUrlManager::BluetoothPath("/api/v2/device/bluetooth");
const std::string CUrlManager::AudioPath("/api/v2/device/audio");
const std::string CUrlManager::NotificationsPath("/api/v2/device/notifications");

CUrlManager::CUrlManager(const CLametricConfiguration& lametricConfiguration)
   : m_lametricConfiguration(lametricConfiguration)
//m_headerParameters(buildHeaderParameters())
{
}

shared::CDataContainer CUrlManager::getState(ERequestType requestType) const
{
   auto protocolType = "http://";
   int port = m_lametricConfiguration.getPort();
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

   auto url = protocolType + ipAddress + ":" + std::to_string(port) + requestPath;

   std::string authorizationHeader = username + ":" + apiKey;


   headerParameters.set("Authorization",
                        AuthorizationType + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");

   return shared::CHttpMethods::sendGetRequest(url,
                                               headerParameters,
                                               parameters);
}

shared::CDataContainer CUrlManager::getDeviceState() const
{
   return getState(kRequestDevice);
}

shared::CDataContainer CUrlManager::getWifiState() const
{
   return getState(kRequestWifi);
}

shared::CDataContainer CUrlManager::getBluetoothState() const
{
   return getState(kRequestBluetooth);
}

shared::CDataContainer CUrlManager::getAudioState() const
{
   return getState(kRequestAudio);
}


shared::CDataContainer CUrlManager::sendPostMessage() const
{
   shared::CDataContainer parameters;

   auto protocolType = "http://";
   int port = m_lametricConfiguration.getPort();
   auto ipAddress = m_lametricConfiguration.getIPAddress();
   auto apiKey = m_lametricConfiguration.getAPIKey();

   std::string username = "dev";

   auto url = protocolType + ipAddress + ":" + std::to_string(port) + NotificationsPath;

   std::string authorizationHeader = username + ":" + apiKey;

   std::string body =
      "{\n\"model\":{\n\"frames\":[\n{\n\"text\":\"Yadoms\",\n\"icon\":\"i31581\",\n\"index\":0\n},\n{\n\"text\":\"yayaya\",\n\"icon\":\"i31581\"\n}\n]\n}\n}";

   std::string AuthorizationType = "Basic ";
   shared::CDataContainer headerParameters;

   headerParameters.set("Authorization",
                        AuthorizationType + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");
   headerParameters.set("Content-Length", body.length());

   return shared::CHttpMethods::sendPostRequest(url,
                                                headerParameters,
                                                parameters,
                                                body);
}
