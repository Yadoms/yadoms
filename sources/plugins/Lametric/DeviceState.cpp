#include "DeviceState.h"
#include "shared/http/HttpMethods.h"


CDeviceState::CDeviceState(CConfiguration& lametricConfiguration)
   : m_lametricConfiguration(lametricConfiguration)
{
}

shared::CDataContainer CDeviceState::getState(const CUrlManagerHelper::ERequestType requestType)
{
   m_urlManagerHelper = boost::make_shared<CUrlManagerHelper>(m_lametricConfiguration);

   const auto requestPath = m_urlManagerHelper->getRequestPath(requestType);

   const auto url = m_urlManagerHelper->getRequestUrl(m_lametricConfiguration, requestPath);

   return shared::CHttpMethods::sendGetRequest(url,
                                               m_urlManagerHelper->buildCommonHeaderParameters(
                                                  m_lametricConfiguration), shared::CDataContainer(),
                                               m_lametricConfiguration.getPort() == kHttp
                                                  ? shared::CHttpMethods::ESessionType::kStandard
                                                  : shared::CHttpMethods::ESessionType::kSecured);
}

shared::CDataContainer CDeviceState::getDeviceState()
{
   return getState(CUrlManagerHelper::kRequestDevice);
}

shared::CDataContainer CDeviceState::getWifiState()
{
   return getState(CUrlManagerHelper::kRequestWifi);
}

shared::CDataContainer CDeviceState::getBluetoothState()
{
   return getState(CUrlManagerHelper::kRequestBluetooth);
}

shared::CDataContainer CDeviceState::getAudioState()
{
   return getState(CUrlManagerHelper::kRequestAudio);
}
