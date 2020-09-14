#include "DeviceState.h"
#include "shared/http/HttpMethods.h"


CDeviceState::CDeviceState(CConfiguration& lametricConfiguration)
	: m_lametricConfiguration(lametricConfiguration)
{
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getState(const CUrlManagerHelper::ERequestType requestType)
{
	const auto url = buildUrl(requestType);

	return shared::http::CHttpMethods::sendJsonGetRequest(
		url,
		m_urlManagerHelper->buildCommonHeaderParameters(m_lametricConfiguration));
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getDeviceInformations()
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

void CDeviceState::getDeviceState()
{
	const auto url = buildUrl(CUrlManagerHelper::kRequestApi);

	shared::http::CHttpMethods::sendHeadRequest(
		url,
		m_urlManagerHelper->buildCommonHeaderParameters(m_lametricConfiguration));
}

std::string CDeviceState::buildUrl(const CUrlManagerHelper::ERequestType requestType)
{
	m_urlManagerHelper = boost::make_shared<CUrlManagerHelper>(m_lametricConfiguration);

	const auto requestPath = m_urlManagerHelper->getRequestPath(requestType);

	return m_urlManagerHelper->getRequestUrl(m_lametricConfiguration, requestPath);
}
