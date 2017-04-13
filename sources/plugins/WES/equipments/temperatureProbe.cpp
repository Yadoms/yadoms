#include "stdafx.h"
#include "temperatureProbe.h"
#include "masterdeviceConfiguration.h"
#include <shared/DataContainer.h>
#include "noInformationException.hpp"
#include "../urlmanager.h"
#include <shared/Log.h>

namespace equipments
{
   CtemperatureProbe::CtemperatureProbe(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const std::string& device,
                                        const shared::CDataContainer& deviceConfiguration,
                                        const boost::shared_ptr<IWESConfiguration> pluginConfiguration
   ):
      m_deviceName(device),
      m_deviceType("temperatureProbe")
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      shared::CDataContainer details;
      details.set("provider", "WES");
      details.set("type", m_deviceType);

      deviceConfiguration.printToLog(YADOMS_LOG(information));

      m_configuration.initializeWith(deviceConfiguration);

      if (pluginConfiguration->isRetrieveNamesFromdevice())
      {
         // request to obtain the name
         std::string CGXfileName = "rel1.cgx";
         shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(),
                                                                  m_configuration.getUser(),
                                                                  m_configuration.getPassword(),
                                                                  CGXfileName);
      }

      m_temperature = boost::make_shared<yApi::historization::CTemperature>("temperature",
                                                                            yApi::EKeywordAccessMode::kGet);

      keywordsToDeclare.push_back(m_temperature);

      //Déclaration of all IOs
      api->declareDevice(device, m_deviceType, keywordsToDeclare, details);
   }

   std::string CtemperatureProbe::getDeviceName() const
   {
      return m_deviceName;
   }

   bool CtemperatureProbe::isMasterDevice() const
   {
      return true;
   }

   std::string CtemperatureProbe::getDeviceType() const
   {
      return m_deviceType;
   }

   void CtemperatureProbe::updateFromDevice( boost::shared_ptr<yApi::IYPluginApi> api,
                                             bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;

      //TODO be modified
      const std::string CGXfileName = "tics.cgx";

      shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(), 
                                                               m_configuration.getUser(),
                                                               m_configuration.getPassword(),
                                                               CGXfileName);

      api->historizeData(m_deviceName, keywordsToHistorize);
   }

   void CtemperatureProbe::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                               shared::CDataContainer& newConfiguration)
   {
      m_configuration.initializeWith(newConfiguration);

      // TODO : If the configuration changed, we need to ask a new time the IP, the password and the user of the master Device
   }

   CtemperatureProbe::~CtemperatureProbe()
   {}
}// namespace equipments