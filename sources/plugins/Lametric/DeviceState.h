#pragma once
#include "shared/DataContainer.h"
#include "Configuration.h"
#include "UrlManagerHelper.h"
#include "IDeviceState.h"

class CDeviceState final : public IDeviceState
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   explicit CDeviceState(CConfiguration& lametricConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CDeviceState() = default;

   // ILametricDeviceState implementation
   shared::CDataContainer getDeviceState() override;
   shared::CDataContainer getWifiState() override;
   shared::CDataContainer getBluetoothState() override;
   shared::CDataContainer getAudioState() override;
   // [END] ILametricDeviceState implementation
private:

   CConfiguration m_lametricConfiguration;

   boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;

   shared::CDataContainer getState(CUrlManagerHelper::ERequestType requestType);
};
