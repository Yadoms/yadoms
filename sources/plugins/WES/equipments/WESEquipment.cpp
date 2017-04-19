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
      std::string relayName[2], TICName[2], ClampName[4], AnalogName[4], inputName[2];
      std::string contract[2];
      std::string PulseType[4];
      std::string PulseName[4];

      m_configuration.initializeWith(deviceConfiguration);
      deviceConfiguration.printToLog(YADOMS_LOG(information));

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

         contract[0] = results.get<std::string>("CPT1_abo_name");
         contract[1] = results.get<std::string>("CPT2_abo_name");

         results.printToLog(YADOMS_LOG(information));

         relayName[0] = results.get<std::string>("RL1");
         relayName[1] = results.get<std::string>("RL2");

         inputName[0] = results.get<std::string>("I1");
         inputName[1] = results.get<std::string>("I2");

         TICName[0] = results.get<std::string>("CPT1_name");
         TICName[1] = results.get<std::string>("CPT2_name");

         ClampName[0] = results.get<std::string>("nPCE1");
         ClampName[1] = results.get<std::string>("nPCE2");
         ClampName[2] = results.get<std::string>("nPCE3");
         ClampName[3] = results.get<std::string>("nPCE4");

         AnalogName[0] = results.get<std::string>("ANA1");
         AnalogName[1] = results.get<std::string>("ANA2");
         AnalogName[2] = results.get<std::string>("ANA3");
         AnalogName[3] = results.get<std::string>("ANA4");

         PulseName[0] = results.get<std::string>("npls1");
         PulseName[1] = results.get<std::string>("npls2");
         PulseName[2] = results.get<std::string>("npls3");
         PulseName[3] = results.get<std::string>("npls4");

         PulseType[0] = results.get<std::string>("PLSU1");
         PulseType[1] = results.get<std::string>("PLSU2");
         PulseType[2] = results.get<std::string>("PLSU3");
         PulseType[3] = results.get<std::string>("PLSU4");
      }
      else  // Defaults names
      {
         contract[0] = "Pas Dispo";
         contract[1] = "Pas Dispo";

         relayName[0] = "R01";
         relayName[1] = "R02";

         inputName[0] = "I1";
         inputName[1] = "I2";

         TICName[0] = "TIC01";
         TICName[1] = "TIC02";

         ClampName[0] = "PCE1";
         ClampName[1] = "PCE2";
         ClampName[2] = "PCE3";
         ClampName[3] = "PCE4";

         AnalogName[0] = "ANA1";
         AnalogName[1] = "ANA2";
         AnalogName[2] = "ANA3";
         AnalogName[3] = "ANA4";

         PulseName[0] = "pls1";
         PulseName[1] = "pls2";
         PulseName[2] = "pls3";
         PulseName[3] = "pls4";

         PulseType[0] = "ND";
         PulseType[1] = "ND";
         PulseType[2] = "ND";
         PulseType[3] = "ND";
      }

      // Relay Configuration
      for (int counter = 0; counter<WES_RELAY_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(relayName[counter],
                                                                                                                 yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Input Configuration
      for (int counter = 0; counter < WES_INPUT_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(inputName[counter],
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_inputList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // TIC Counters Configuration
      for (int counter = 0; counter<WES_TIC_QTY; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CTIC> temp = boost::make_shared<equipments::subdevices::CTIC>(api, m_deviceName + " - " + TICName[counter], contract[counter]);
         m_TICList.push_back(temp);
      }

      // Pulse Counters Configuration
      for (int counter = 0; counter<WES_PULSE_QTY; ++counter)
      {
         // TODO : Create a function to analyze the type and create the correct one
         boost::shared_ptr<equipments::subdevices::CPulse> temp = boost::make_shared<equipments::subdevices::CPulse>(api,
                                                                                                                     keywordsToDeclare,
                                                                                                                     m_deviceName,
                                                                                                                     PulseName[counter],
                                                                                                                     PulseType[counter]);
         m_PulseList.push_back(temp);
      }

      // Current clamp Configuration
      for (int counter = 0; counter<WES_CLAMP_QTY; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CClamp> temp = boost::make_shared<equipments::subdevices::CClamp>(api,
                                                                                                                     keywordsToDeclare,
                                                                                                                     pluginConfiguration,
                                                                                                                     m_configuration.isInstantCurrentClampRegistered(counter),
                                                                                                                     m_deviceName,
                                                                                                                     ClampName[counter]);
         m_ClampList.push_back(temp);
      }

      // Analog Values Configuration
      for (int counter = 0; counter<WES_ANA_QTY; ++counter)
      {
         boost::shared_ptr<specificHistorizers::CAnalog> temp = boost::make_shared<specificHistorizers::CAnalog>(AnalogName[counter],
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
                                         const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                                         bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;
      std::string CGXfileName = "WESVALUES.CGX";

      shared::CDataContainer credentials;

      // TODO : To be replaced when manual configuration device is properly finished !
      credentials.set("user", "admin"/*m_configuration.getUser()*/);
      credentials.set("password", "wes"/*m_configuration.getPassword()*/);

      credentials.printToLog(YADOMS_LOG(information));

      shared::CDataContainer results = urlManager::sendCommand(m_configuration.getIPAddressWithSocket(), 
                                                               credentials,
                                                               CGXfileName);

      // Pulse Counters Configuration
      //createUpdatePulsesKeywords(keywordsToDeclare, results, pluginConfiguration);

      // Reading relays - historize only on change value or when the historization is forced (initialization, for example)      
      updateSwitchValue(keywordsToHistorize, m_relaysList[0], static_cast<bool>(results.get<int>("RL1")), forceHistorization);
      updateSwitchValue(keywordsToHistorize, m_relaysList[1], static_cast<bool>(results.get<int>("RL2")), forceHistorization);

      updateSwitchValue(keywordsToHistorize, m_inputList[0], static_cast<bool>(results.get<int>("I1")), forceHistorization);
      updateSwitchValue(keywordsToHistorize, m_inputList[1], static_cast<bool>(results.get<int>("I2")), forceHistorization);

      api->historizeData(m_deviceName, keywordsToHistorize);
   }

   void CWESEquipment::updateSwitchValue(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize, 
                                         boost::shared_ptr<yApi::historization::CSwitch> keyword, 
                                         bool newValue, 
                                         bool forceHistorization)
   {
      if (newValue != keyword->get() || forceHistorization)
      {
         keyword->set(newValue);
         keywordsToHistorize.push_back(keyword);

         YADOMS_LOG(trace) << keyword->getKeyword() << " set to " << newValue;
      }
   }

   void CWESEquipment::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                           shared::CDataContainer& newConfiguration)
   {
      m_configuration.initializeWith(newConfiguration);

      // TODO : Change the configuration, if any
   }

   /*
   tu peux controler les relais en UDP, TCP ou par des requêtes HTTP:
   Par une requête HTTP :
   http://WES/RL.cgi?rl1=ON&rl2=OFF
   Vous pouvez remplacer WES par l'adresse IP du serveur.
   Si votre navigateur n'est pas logé (admin et mot de passe envoyé au serveur) vous devez les rajouter à la requête HTTP :
   http://user:password@WES/RL.cgx?rl1=ON&rl2=OFF
   Soit avec les paramètres d'origine du serveur :
   http://admin:wes@WES/RL.cgx?rl1=ON&rl2=OFF
   Pour activer un relais d'une carte 1WIRE il suffit de donner le numéro du relais :
   http://WES/RL.cgx?rl111=ON
   Alors le relais 111 sera activé, il correspond à la pompe de la piscine.
   Vous pouvez aussi agir sur tous les relais d'une carte 1WIRE en commandant le relais virtuel numéro 9 de chaque carte :
   http://WES/RL.cgx?rl119=OFF
   */

   CWESEquipment::~CWESEquipment()
   {
      //TODO : Check how to destroy subModules -> A specific function should be done
   }
}// namespace equipments