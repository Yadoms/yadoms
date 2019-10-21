#pragma once
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>

class IUrlManager
{
public:
   virtual ~IUrlManager() = default;

   virtual shared::CDataContainer getDeviceState(const std::string& ipAddress, const std::string& port,
                                                 const std::string& apikey) = 0;

   virtual shared::CDataContainer getWifiState(const std::string& ipAddress, const std::string& port,
                                               const std::string& apikey) = 0;

   virtual shared::CDataContainer getBluetoothState(const std::string& ipAddress, const std::string& port,
                                                    const std::string& apikey) = 0;

   virtual shared::CDataContainer getAudioState(const std::string& ipAddress, const std::string& port,
                                               const std::string& apikey) = 0;
};
