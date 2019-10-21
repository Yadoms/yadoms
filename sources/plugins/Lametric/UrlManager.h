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

   shared::CDataContainer CUrlManager::getDeviceState() override;

   shared::CDataContainer CUrlManager::getWifiState() override;

   shared::CDataContainer CUrlManager::getBluetoothState() override;

   shared::CDataContainer CUrlManager::getAudioState() override;

private:

   static const std::string m_devicePath;
   static const std::string m_WifiPath;
   static const std::string m_BluetoothPath;
   static const std::string m_audioPath;
   CLametricConfiguration m_lametricConfiguration;

};
