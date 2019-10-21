#include "UrlManager.h"
#include "shared/http/HttpMethods.h"
#include "shared/encryption/Base64.h"
#include "shared/http/StandardSession.h"
#include <boost/make_shared.hpp>

const std::string CUrlManager::m_devicePath("/api/v2/device");
const std::string CUrlManager::m_WifiPath("/api/v2/device/wifi");
const std::string CUrlManager::m_BluetoothPath("/api/v2/device/bluetooth");
const std::string CUrlManager::m_audioPath("/api/v2/device/audio");

CUrlManager::CUrlManager(const CLametricConfiguration& lametricConfiguration)
{
   m_lametricConfiguration = lametricConfiguration;
}

shared::CDataContainer CUrlManager::getDeviceState()
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + m_lametricConfiguration.getIPAddress() + ":" + m_lametricConfiguration.getPort() +
      m_devicePath;

   std::string authorizationHeader = "dev:" + m_lametricConfiguration.getAPIKey();

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

shared::CDataContainer CUrlManager::getWifiState()
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + m_lametricConfiguration.getIPAddress() + ":" + m_lametricConfiguration.getPort() +
      m_WifiPath;

   std::string authorizationHeader = "dev:" + m_lametricConfiguration.getAPIKey();

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


shared::CDataContainer CUrlManager::getBluetoothState()
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + m_lametricConfiguration.getIPAddress() + ":" + m_lametricConfiguration.getPort() +
      m_BluetoothPath;

   std::string authorizationHeader = "dev:" + m_lametricConfiguration.getAPIKey();

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


shared::CDataContainer CUrlManager::getAudioState()
{
   shared::CDataContainer response;
   shared::CDataContainer headerParameters;
   shared::CDataContainer parameters;

   //format url
   auto uriStr = "http://" + m_lametricConfiguration.getIPAddress() + ":" + m_lametricConfiguration.getPort() +
      m_audioPath;

   std::string authorizationHeader = "dev:" + m_lametricConfiguration.getAPIKey();

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
