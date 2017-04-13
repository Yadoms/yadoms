#include "stdafx.h"
#include "WESEquipment.h"
#include "masterdeviceConfiguration.h"
#include <shared/DataContainer.h>
#include "noInformationException.hpp"
#include "../urlmanager.h"
#include <shared/Log.h>

namespace equipments
{
   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const shared::CDataContainer& deviceConfiguration,
                                const boost::shared_ptr<IWESConfiguration> pluginConfiguration
   ):
      m_deviceName(device),
      m_deviceType("WES")
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      shared::CDataContainer details;
      details.set("provider", "WES");
      details.set("type", m_deviceType);

      deviceConfiguration.printToLog(YADOMS_LOG(information));

      m_configuration.initializeWith(deviceConfiguration);

      if (pluginConfiguration->isRetrieveNamesFromdevice())
      {
         // request to obtain names
         std::string CGXfileName = "rel1.cgx";
         shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(),
                                                                  m_configuration.getUser(),
                                                                  m_configuration.getPassword(),
                                                                  CGXfileName);
      }

      // Relay Configuration
      for (int counter = 0; counter<WES_RELAY_QTY; ++counter)
      {
         std::stringstream name;
         name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(),
                                                                                                                 yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // TIC Counters Configuration
      for (int counter = 0; counter<WES_TIC_QTY; ++counter)
      {
         std::stringstream name;
         name << "TIC" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CEnergy> temp = boost::make_shared<yApi::historization::CEnergy>(name.str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_counterTICList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Pulse Counters Configuration
      for (int counter = 0; counter<WES_PULSE_QTY; ++counter)
      {
         std::stringstream name;
         name << "IMP" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(name.str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_PulseCounterList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      //Déclaration of all IOs
      api->declareDevice(device, m_deviceType, keywordsToDeclare, details);
   }

   std::string CWESEquipment::getDeviceName() const
   {
      return m_deviceName;
   }

   bool CWESEquipment::isMasterDevice() const
   {
      return true;
   }

   std::string CWESEquipment::getDeviceType() const
   {
      return m_deviceType;
   }

   void CWESEquipment::updateFromDevice( boost::shared_ptr<yApi::IYPluginApi> api,
                                         bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;
      std::string CGXfileName = "tics.cgx";

      shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(), 
                                                               m_configuration.getUser(),
                                                               m_configuration.getPassword(),
                                                               CGXfileName);

      api->historizeData(m_deviceName, keywordsToHistorize);
   }

   void CWESEquipment::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                            shared::CDataContainer& newConfiguration)
   {
      m_configuration.initializeWith(newConfiguration);
   }

   CWESEquipment::~CWESEquipment()
   {}
}// namespace equipments