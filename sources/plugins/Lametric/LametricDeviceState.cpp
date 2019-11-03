#include "LametricDeviceState.h"
#include "shared/http/HttpMethods.h"


CLametricDeviceState::CLametricDeviceState(CLametricConfiguration& lametricConfiguration)
   :m_lametricConfiguration(lametricConfiguration)
{
}

shared::CDataContainer CLametricDeviceState::getState(const CUrlManagerHelper::ERequestType requestType)
{
   std::string authorizationType = "Basic ";

   m_urlManagerHelper = boost::make_shared<CUrlManagerHelper>(m_lametricConfiguration);

   const auto requestPath = m_urlManagerHelper->getRequestPath(requestType);


   const auto url = m_urlManagerHelper->getUrl(m_lametricConfiguration, requestPath);

   return shared::CHttpMethods::sendGetRequest(url,
      m_urlManagerHelper->buildCommonHeaderParameters(m_lametricConfiguration));
}

shared::CDataContainer CLametricDeviceState::getDeviceState() 
{
   return getState(CUrlManagerHelper::kRequestDevice);
}

shared::CDataContainer CLametricDeviceState::getWifiState() 
{
   return getState(CUrlManagerHelper::kRequestWifi);
}

shared::CDataContainer CLametricDeviceState::getBluetoothState() 
{
   return getState(CUrlManagerHelper::kRequestBluetooth);
}

shared::CDataContainer CLametricDeviceState::getAudioState() 
{
   return getState(CUrlManagerHelper::kRequestAudio);
}