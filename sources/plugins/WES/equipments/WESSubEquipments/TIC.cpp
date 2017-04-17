#include "stdafx.h"
#include "TIC.h"
#include <shared/Log.h>

namespace equipments
{
   CTIC::CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& name,
                                const shared::CDataContainer& deviceConfiguration
   ):
      m_deviceName(name)
   {
/*
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      std::string relayName[2], TICName[2], PulseName[4], ClampName[4], AnalogName[4];

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
         // TODO : To be created when received a WESVALUES Frame
         // TODO : Create a function to analyze the contract and create all needed counters
         // TODO : Create all the necessary for counters period time, couleur, puissance apparente
         boost::shared_ptr<yApi::historization::CEnergy> temp = boost::make_shared<yApi::historization::CEnergy>(TICName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_counterTICList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Pulse Counters Configuration
      for (int counter = 0; counter<WES_PULSE_QTY; ++counter)
      {
         // TODO : To be created when received a WESVALUES Frame
         // TODO : Create a function to analyze the type and create the correct one
         boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(PulseName[counter].c_str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_PulseCounterList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Current clamp Configuration
      for (int counter = 0; counter<WES_CLAMP_QTY; ++counter)
      {
         // TODO : Create a function to analyze the type and create the correct one
         boost::shared_ptr<yApi::historization::CCurrent> tempCurrent = boost::make_shared<yApi::historization::CCurrent>("I - " + ClampName[counter],
                                                                                                                   yApi::EKeywordAccessMode::kGet);

         boost::shared_ptr<yApi::historization::CEnergy>  tempCounter = boost::make_shared<yApi::historization::CEnergy>("Wh - " + ClampName[counter],
                                                                                                                   yApi::EKeywordAccessMode::kGet);

         m_CurrentClampList.push_back(tempCurrent);
         m_CounterClampList.push_back(tempCounter);
         keywordsToDeclare.push_back(tempCurrent);
         keywordsToDeclare.push_back(tempCounter);
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
*/
   }

   std::string CTIC::getDeviceName() const
   {
      return m_deviceName;
   }

   void CTIC::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;
      std::string CGXfileName = "WESVALUES.CGX";
   }

   CTIC::~CTIC()
   {
      //TODO : Check how to destroy subModules
   }
}// namespace equipments