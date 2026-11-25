#include <boost/format.hpp>
#include "DeviceState.h"
#include "shared/http/HttpRestHelpers.h"
#include "shared/Log.h"

CDeviceState::CDeviceState(CConfiguration& lametricConfiguration)
   : m_lametricConfiguration(lametricConfiguration)
{
}

boost::shared_ptr<shared::CDataContainer> CDeviceState::getState(const CUrlManagerHelper::ERequestType requestType)
{
   const auto url = buildUrl(requestType);

   boost::shared_ptr<shared::CDataContainer> response;
   shared::http::CHttpRestHelpers::createHttpRestRequest(shared::http::ERestVerb::kGet, url)
      ->withHeaderParameters(m_urlManagerHelper->buildCommonHeaderParameters())
      .withBasicAuthentication("Basic", m_lametricConfiguration.getAPIKey())
      .send([&response](auto data)
      {
         response = std::move(data);
      });

   return response;
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

   boost::shared_ptr<shared::CDataContainer> response;
   try
   {
      shared::http::CHttpRestHelpers::createHttpRestRequest(shared::http::ERestVerb::kHead, url)
         ->withHeaderParameters(m_urlManagerHelper->buildCommonHeaderParameters())
         .withBasicAuthentication("Basic", m_lametricConfiguration.getAPIKey())
         .send([&response](auto data)
         {
            response = data;
         });
   }
   catch (std::exception& e)
   {
      const auto message = (boost::format("Fail to send Head http request or interpret answer \"%1%\" : %2%") % url %
         e.what()).str();
      YADOMS_LOG(error) << "Fail to send Head http request or interpret answer " << url << " : " << e.what();
   }
}

std::string CDeviceState::buildUrl(const CUrlManagerHelper::ERequestType requestType)
{
   m_urlManagerHelper = boost::make_shared<CUrlManagerHelper>(m_lametricConfiguration);

   const auto requestPath = m_urlManagerHelper->getRequestPath(requestType);

   return m_urlManagerHelper->getRequestUrl(m_lametricConfiguration, requestPath);
}
