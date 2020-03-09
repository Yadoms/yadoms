#pragma once
#include "shared/DataContainer.h"
#include "Configuration.h"
#include "UrlManagerHelper.h"
#include "IDeviceState.h"

class CDeviceState : public IDeviceState
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
   shared::CDataContainerSharedPtr getDeviceState() override;
   shared::CDataContainerSharedPtr getWifiState() override;
   shared::CDataContainerSharedPtr getBluetoothState() override;
   shared::CDataContainerSharedPtr getAudioState() override;
   // [END] ILametricDeviceState implementation
private:

   CConfiguration m_lametricConfiguration;

   boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;

   shared::CDataContainerSharedPtr getState(CUrlManagerHelper::ERequestType requestType);
};
