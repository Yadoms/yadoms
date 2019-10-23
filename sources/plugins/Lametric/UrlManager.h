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

   shared::CDataContainer getState(ERequestType requestType);

private:

   static const std::string DevicePath;
   static const std::string WifiPath;
   static const std::string BluetoothPath;
   static const std::string AudioPath;

   const CLametricConfiguration& m_lametricConfiguration;
};
