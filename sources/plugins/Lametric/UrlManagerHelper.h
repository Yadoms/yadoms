#pragma once
#include <string>
#include "LametricConfiguration.h"


class CUrlManagerHelper
{
public:

   CUrlManagerHelper(CLametricConfiguration& lametricConfiguration);

   virtual ~CUrlManagerHelper() = default;

   enum ERequestType
   {
      kRequestDevice,
      kRequestWifi,
      kRequestBluetooth,
      kRequestAudio,
      kRequestNotivications,
      kRequestUnknown
   };

   static std::string getRequestPath(ERequestType requestType);
   static std::string getUrl(const CLametricConfiguration& lametricConfiguration, const std::string& requestPath);
   static shared::CDataContainer buildCommonHeaderParameters(const CLametricConfiguration& lametricConfiguration);

private:
   const CLametricConfiguration& m_lametricConfiguration;
   const shared::CDataContainer m_commonHeaderParameters;
   static const std::string DevicePath;
   static const std::string WifiPath;
   static const std::string BluetoothPath;
   static const std::string AudioPath;
   static const std::string NotificationsPath;
   static const std::string Username;
};
