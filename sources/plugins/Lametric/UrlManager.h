#pragma once
#include "shared/DataContainer.h"
#include "IUrlManager.h"

class CUrlManager : public IUrlManager
{
public:

   CUrlManager() = default;

   virtual ~CUrlManager() = default;

   shared::CDataContainer CUrlManager::getDeviceState(const std::string& ipAddress, const std::string& port,
                                                      const std::string& apikey) override;

   shared::CDataContainer CUrlManager::getWifiState(const std::string& ipAddress, const std::string& port,
                                                    const std::string& apikey) override;

   shared::CDataContainer CUrlManager::getBluetoothState(const std::string& ipAddress, const std::string& port,
                                                         const std::string& apikey) override;

   shared::CDataContainer CUrlManager::getAudioState(const std::string& ipAddress, const std::string& port,
                                                     const std::string& apikey) override;
};
