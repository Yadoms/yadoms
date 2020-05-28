#include "UrlManagerHelper.h"
#include "shared/encryption/Base64.h"


const std::string CUrlManagerHelper::DevicePath("/api/v2/device");
const std::string CUrlManagerHelper::WifiPath("/api/v2/device/wifi");
const std::string CUrlManagerHelper::BluetoothPath("/api/v2/device/bluetooth");
const std::string CUrlManagerHelper::AudioPath("/api/v2/device/audio");
const std::string CUrlManagerHelper::NotificationsPath("/api/v2/device/notifications");
const std::string CUrlManagerHelper::Username("dev");

CUrlManagerHelper::CUrlManagerHelper(CConfiguration& lametricConfiguration)
   : m_lametricConfiguration(lametricConfiguration),
     m_commonHeaderParameters(buildCommonHeaderParameters(lametricConfiguration))
{
}

std::string CUrlManagerHelper::getRequestPath(const ERequestType requestType)
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
   case kRequestNotifications:
      requestPath = NotificationsPath;
      break;
   default:
      requestPath = "";;
   }
   return requestPath;
}

std::string CUrlManagerHelper::getRequestUrl(const CConfiguration& lametricConfiguration,
                                      const std::string& requestPath)
{
   const auto protocolType = lametricConfiguration.getPort() == kHttp ? "http://" : "https://";

   return protocolType + lametricConfiguration.getIPAddress() + ":" + std::to_string(lametricConfiguration.getPort()) +
      requestPath;
}


boost::shared_ptr<shared::CDataContainer> CUrlManagerHelper::buildCommonHeaderParameters(const CConfiguration& lametricConfiguration)
{
   const auto apiKey = lametricConfiguration.getAPIKey();

   const std::string authorizationType = "Basic ";

   const auto authorizationHeader = Username + ":" + apiKey;


   boost::shared_ptr<shared::CDataContainer> headerParameters = shared::CDataContainer::make();
   headerParameters->set("Authorization",
                        authorizationType + shared::encryption::CBase64::encode(
                           reinterpret_cast<const unsigned char*>(authorizationHeader.c_str()),
                           authorizationHeader.length()));

   headerParameters->set("User-Agent", "yadoms");
   headerParameters->set("Accept", "application/json");
   headerParameters->set("Connection", "close");

   return headerParameters;
}
