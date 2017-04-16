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
      std::string relayName[2], TICName[2], PulseName[4], ClampName[4];

      deviceConfiguration.printToLog(YADOMS_LOG(information));

      m_configuration.initializeWith(deviceConfiguration);

      if (pluginConfiguration->isRetrieveNamesFromdevice())
      {
         // request to obtain names
         std::string CGXfileName = "WESNAMES.CGX";
         shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(),
                                                                  m_configuration.getUser(),
                                                                  m_configuration.getPassword(),
                                                                  CGXfileName);

         results.printToLog(YADOMS_LOG(information));

         relayName[0] = results.get<std::string>("RL1");
         relayName[1] = results.get<std::string>("RL2");

         TICName[0] = results.get<std::string>("CPT1_name");
         TICName[1] = results.get<std::string>("CPT2_name");

         PulseName[0] = results.get<std::string>("npls1");
         PulseName[1] = results.get<std::string>("npls2");
         PulseName[2] = results.get<std::string>("npls3");
         PulseName[3] = results.get<std::string>("npls4");

         ClampName[0] = results.get<std::string>("nPCE1");
         ClampName[1] = results.get<std::string>("nPCE2");
         ClampName[2] = results.get<std::string>("nPCE3");
         ClampName[3] = results.get<std::string>("nPCE4");
      }
      else  // Defaults names
      {
         relayName[0] = "R01";
         relayName[1] = "R02";

         TICName[0] = "TIC01";
         TICName[1] = "TIC02";

         PulseName[0] = "pls1";
         PulseName[1] = "pls2";
         PulseName[2] = "pls3";
         PulseName[3] = "pls4";

         ClampName[0] = "PCE1";
         ClampName[1] = "PCE2";
         ClampName[2] = "PCE3";
         ClampName[3] = "PCE4";
      }

      // Relay Configuration
      for (int counter = 0; counter<WES_RELAY_QTY; ++counter)
      {
         //std::stringstream name;
         //name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(relayName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // TIC Counters Configuration
      for (int counter = 0; counter<WES_TIC_QTY; ++counter)
      {
         //std::stringstream name;
         //name << "TIC" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CEnergy> temp = boost::make_shared<yApi::historization::CEnergy>(TICName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_counterTICList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Pulse Counters Configuration
      for (int counter = 0; counter<WES_PULSE_QTY; ++counter)
      {
         //std::stringstream name;
         //name << "IMP" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(PulseName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_PulseCounterList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Current clamp Configuration
      for (int counter = 0; counter<WES_CLAMP_QTY; ++counter)
      {
         std::stringstream name;
         name << "Current Clamp" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(ClampName[counter].c_str(),
                                                                                                                   yApi::EKeywordAccessMode::kGet);
         m_CurrentClampList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      shared::CDataContainer details;
      details.set("provider", "WES");
      details.set("type", m_deviceType);

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