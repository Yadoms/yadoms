#include "DeviceState.h"
#include "shared/http/HttpMethods.h"


CDeviceState::CDeviceState(CConfiguration& lametricConfiguration)
   : m_lametricConfiguration(lametricConfiguration)
{
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getState(const CUrlManagerHelper::ERequestType requestType)
{
   m_urlManagerHelper = boost::make_shared<CUrlManagerHelper>(m_lametricConfiguration);

   const auto requestPath = m_urlManagerHelper->getRequestPath(requestType);

   const auto url = m_urlManagerHelper->getRequestUrl(m_lametricConfiguration, requestPath);

   return shared::CHttpMethods::sendJsonGetRequest(
      url,
      m_urlManagerHelper->buildCommonHeaderParameters(m_lametricConfiguration),
      std::map<std::string, std::string>(),
      m_lametricConfiguration.getPort() == kHttp
         ? shared::CHttpMethods::ESessionType::kStandard
         : shared::CHttpMethods::ESessionType::kSecured);
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getDeviceState()
{
   return getState(CUrlManagerHelper::kRequestDevice);
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getWifiState()
{
   return getState(CUrlManagerHelper::kRequestWifi);
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getBluetoothState()
{
   return getState(CUrlManagerHelper::kRequestBluetooth);
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getAudioState()
{
   return getState(CUrlManagerHelper::kRequestAudio);
}
