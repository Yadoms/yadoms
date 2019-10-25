#pragma once
#include "shared/DataContainer.h"
#include "IUrlManager.h"
#include "LametricConfiguration.h"

class CUrlManager
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
      kRequestUnknown
   };

   shared::CDataContainer getState(ERequestType requestType) const;
   shared::CDataContainer getDeviceState() const;
   shared::CDataContainer getWifiState() const;
   shared::CDataContainer getBluetoothState() const;
   shared::CDataContainer getAudioState() const;

   shared::CDataContainer sendPostMessage() const;


private:

   static const std::string DevicePath;
   static const std::string WifiPath;
   static const std::string BluetoothPath;
   static const std::string AudioPath;
   static const std::string NotificationsPath;
   const CLametricConfiguration& m_lametricConfiguration;
};
