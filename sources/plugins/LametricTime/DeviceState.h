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
   boost::shared_ptr<shared::CDataContainer> getDeviceInformations() override;
   boost::shared_ptr<shared::CDataContainer> getWifiState() override;
   boost::shared_ptr<shared::CDataContainer> getBluetoothState() override;
   boost::shared_ptr<shared::CDataContainer> getAudioState() override;
   void getDeviceState() override;
   // [END] ILametricDeviceState implementation
private:

   CConfiguration m_lametricConfiguration;

   boost::shared_ptr<CUrlManagerHelper> m_urlManagerHelper;

   boost::shared_ptr<shared::CDataContainer> getState(CUrlManagerHelper::ERequestType requestType);

   std::string CDeviceState::buildUrl(const CUrlManagerHelper::ERequestType requestType);
};
