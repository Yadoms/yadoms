#include "UrlManager.h"
#include "shared/http/HttpMethods.h"
#include "shared/encryption/Base64.h"
#include "shared/http/StandardSession.h"
#include "shared/http/HttpException.hpp"
#include "shared/Log.h"

const std::string CUrlManager::DevicePath("/api/v2/device");
const std::string CUrlManager::WifiPath("/api/v2/device/wifi");
const std::string CUrlManager::BluetoothPath("/api/v2/device/bluetooth");
const std::string CUrlManager::AudioPath("/api/v2/device/audio");
const std::string CUrlManager::NotificationsPath("/api/v2/device/notifications");
const std::string CUrlManager::Username("dev");


CUrlManager::CUrlManager(const CLametricConfiguration& lametricConfiguration)
   : m_lametricConfiguration(lametricConfiguration),
     m_headerParameters(buildHeaderParameters(lametricConfiguration))
{
}

shared::CDataContainer CUrlManager::getState(const ERequestType requestType) const
{
   std::string authorizationType = "Basic ";

   const auto requestPath = getRequestPath(requestType);


   const auto url = getUrl(m_lametricConfiguration, requestPath);

   return shared::CHttpMethods::sendGetRequest(url,
                                               m_headerParameters);
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

void CUrlManager::displayText(const std::string& text,
                              notificationState::CNotificationPriority::EPriorityType priorityType,
                              notificationState::CNotificationIcon::EIconType iconType) const
{
   std::string authorizationType = "Basic ";
   std::string priorityMessage;
   std::string iconToDisplay;

   notificationState::CNotificationPriority::getPriorityType(priorityType, priorityMessage);
   notificationState::CNotificationIcon::getIconType(iconType, iconToDisplay);
   const auto requestPath = getRequestPath(kRequestNotivications);

   const auto url = getUrl(m_lametricConfiguration, requestPath);
   const std::string body =
      "{\n\"priority\": " + priorityMessage + "{\n\"icon_type\": " + iconToDisplay +
      "\n\"model\":{\n\"frames\":[\n{\n\"text\":\"Yadoms\",\n\"icon\":\"i31581\",\n\"index\":0\n},\n{\n\"text\":\"" +
      text + "\",\n\"icon\":\"i31581\"\n}\n]\n}\n}";

   auto headerPostParameters = m_headerParameters;
   headerPostParameters.set("Content-Length", body.length());

   try
   {
      shared::CHttpMethods::sendPostRequest(url,
                                            headerPostParameters,
                                            shared::CDataContainer(),
                                            body);
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Post http request or interpret answer \"%1%\" : %2%") % url %
         e.what()).str();
      YADOMS_LOG(error) << message;
      throw shared::CHttpException(message);
   }
}

shared::CDataContainer CUrlManager::buildHeaderParameters(const CLametricConfiguration& lametricConfiguration)
{
   const auto apiKey = lametricConfiguration.getAPIKey();

   const std::string authorizationType = "Basic ";

   const auto authorizationHeader = Username + ":" + apiKey;


   shared::CDataContainer headerParameters;
   headerParameters.set("Authorization",
                        authorizationType + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters.set("User-Agent", "yadoms");
   headerParameters.set("Accept", "application/json");
   headerParameters.set("Connection", "close");

   return headerParameters;
}

std::string CUrlManager::getRequestPath(const ERequestType requestType)
{
   std::string requestPath;
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
   case kRequestNotivications:
      requestPath = NotificationsPath;
      break;
   default:
      requestPath = "";;
   }
   return requestPath;
}

std::string CUrlManager::getUrl(const CLametricConfiguration& lametricConfiguration, const std::string& requestPath)
{
   const auto protocolType = lametricConfiguration.getPort() == kHttp ? "http://" : "https://";

   return protocolType + lametricConfiguration.getIPAddress() + ":" + std::to_string(lametricConfiguration.getPort()) +
      requestPath;
}
