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
      std::string relayName[2], TICName[2], PulseName[4], ClampName[4], AnalogName[4];

      deviceConfiguration.printToLog(YADOMS_LOG(information));

      m_configuration.initializeWith(deviceConfiguration);

      if (pluginConfiguration->isRetrieveNamesFromdevice())
      {
         shared::CDataContainer credentials;

         // TODO : To be replaced when manual configuration device is properly finished !
         credentials.set("user", "admin"/*m_configuration.getUser()*/);
         credentials.set("password", "wes"/*m_configuration.getPassword()*/);

         credentials.printToLog(YADOMS_LOG(information));

         // request to obtain names
         std::string CGXfileName = "WESNAMES.CGX";
         shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(),
                                                                  credentials,
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

         AnalogName[0] = results.get<std::string>("ANA1");
         AnalogName[1] = results.get<std::string>("ANA2");
         AnalogName[2] = results.get<std::string>("ANA3");
         AnalogName[3] = results.get<std::string>("ANA4");
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

         AnalogName[0] = "ANA1";
         AnalogName[1] = "ANA2";
         AnalogName[2] = "ANA3";
         AnalogName[3] = "ANA4";
      }

      // Relay Configuration
      for (int counter = 0; counter<WES_RELAY_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(relayName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // TIC Counters Configuration
      for (int counter = 0; counter<WES_TIC_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CEnergy> temp = boost::make_shared<yApi::historization::CEnergy>(TICName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_counterTICList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Pulse Counters Configuration
      for (int counter = 0; counter<WES_PULSE_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(PulseName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_PulseCounterList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Current clamp Configuration
      for (int counter = 0; counter<WES_CLAMP_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CCurrent> temp = boost::make_shared<yApi::historization::CCurrent>(ClampName[counter].c_str(),
                                                                                                                   yApi::EKeywordAccessMode::kGet);
         m_CurrentClampList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Analog Values Configuration
      for (int counter = 0; counter<WES_ANA_QTY; ++counter)
      {
         boost::shared_ptr<specificHistorizers::CAnalog> temp = boost::make_shared<specificHistorizers::CAnalog>(AnalogName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_AnalogList.push_back(temp);
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

      shared::CDataContainer credentials;

      // TODO : To be replaced when manual configuration device is properly finished !
      credentials.set("user", "admin"/*m_configuration.getUser()*/);
      credentials.set("password", "wes"/*m_configuration.getPassword()*/);

      credentials.printToLog(YADOMS_LOG(information));

      shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(), 
                                                               credentials,
                                                               CGXfileName);

      api->historizeData(m_deviceName, keywordsToHistorize);
   }

   void CWESEquipment::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                            shared::CDataContainer& newConfiguration)
   {
      m_configuration.initializeWith(newConfiguration);
   }

   CWESEquipment::~CWESEquipment()
   {
      //TODO : Check how to destroy subModules
   }
}// namespace equipments