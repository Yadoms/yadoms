#pragma once
#include "shared/DataContainer.h"
#include "LametricConfiguration.h"
#include "UrlManagerHelper.h"
#include "ILametricDeviceState.h"

class CLametricDeviceState : ILametricDeviceState
{
public:

   CLametricDeviceState(CLametricConfiguration& lametricConfiguration);

   virtual ~CLametricDeviceState() = default;

   // ILametricDeviceState implementation
   shared::CDataContainer getDeviceState() override;
   shared::CDataContainer getWifiState() override;
   shared::CDataContainer getBluetoothState() override;
   shared::CDataContainer getAudioState() override;
   // [END] ILametricDeviceState implementation
private:

   CLametricConfiguration m_lametricConfiguration;

   boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;

   shared::CDataContainer getState(CUrlManagerHelper::ERequestType requestType);
};
