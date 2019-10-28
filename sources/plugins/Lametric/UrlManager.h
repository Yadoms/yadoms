#pragma once
#include "shared/DataContainer.h"
#include "IUrlManager.h"
#include "LametricConfiguration.h"

class CUrlManager : public IUrlManager
{
public:

   CUrlManager() = default;

   CUrlManager(const CLametricConfiguration& lametricConfiguration);

   virtual ~CUrlManager() = default;

   enum ERequestType
   {
      kRequestDevice,
      kRequestWifi,
      kRequestBluetooth,
      kRequestAudio,
      kRequestNotivications,
      kRequestUnknown
   };

   // IUrlManagerinterface implementation
   shared::CDataContainer getDeviceState() const override;
   shared::CDataContainer getWifiState() const override;
   shared::CDataContainer getBluetoothState() const override;
   shared::CDataContainer getAudioState() const override;
   void displayText(const std::string& text) const override;
   // [END] IUrlManagerinterface implementation


private:

   static const std::string DevicePath;
   static const std::string WifiPath;
   static const std::string BluetoothPath;
   static const std::string AudioPath;
   static const std::string NotificationsPath;
   static const std::string Username;
   const CLametricConfiguration& m_lametricConfiguration;
   const shared::CDataContainer m_headerParameters;

   static std::string getRequestPath(const ERequestType requestType);
   static std::string getUrl(const CLametricConfiguration& lametricConfiguration, const std::string& requestPath);
   shared::CDataContainer getState(ERequestType requestType) const;
   static shared::CDataContainer buildHeaderParameters(const CLametricConfiguration& lametricConfiguration);
};
