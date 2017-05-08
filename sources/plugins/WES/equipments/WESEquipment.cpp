#include "stdafx.h"
#include "WESEquipment.h"
#include <shared/DataContainer.h>
#include "noInformationException.hpp"
#include "../urlManager.h"
#include <shared/Log.h>

namespace equipments
{
   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const shared::CDataContainer& deviceConfiguration) :
      m_deviceName(device),
      m_deviceType("WES"),
      m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("Status"))
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

      m_deviceStatus->set(specificHistorizers::EdeviceStatus::kUndefined);
      keywordsToDeclare.push_back(m_deviceStatus);

      m_configuration.initializeWith(deviceConfiguration);
      shared::CDataContainer details = api->getDeviceDetails(device);

      shared::CDataContainer TICContainerName = details.get<shared::CDataContainer>("TIC");
      for (int counter = 0; counter < WES_TIC_QTY; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CTIC> temp = boost::make_shared<equipments::subdevices::CTIC>(api, 
                                                                                                                  TICContainerName.get<std::string>("TIC" + boost::lexical_cast<std::string>(counter)),
                                                                                                                  TICContainerName.get<std::string>("contract" + boost::lexical_cast<std::string>(counter)));
         m_TICList.push_back(temp);
      }

      shared::CDataContainer RelayContainerName = details.get<shared::CDataContainer>("Relays");
      for (int counter = 0; counter < WES_RELAY_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(RelayContainerName.get<std::string>("R" + boost::lexical_cast<std::string>(counter)),
                                                                                                                  yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      shared::CDataContainer inputContainerName = details.get<shared::CDataContainer>("Inputs");
      for (int counter = 0; counter < WES_INPUT_QTY; ++counter)
      {
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(inputContainerName.get<std::string>("I" + boost::lexical_cast<std::string>(counter)),
                                                                                                                  yApi::EKeywordAccessMode::kGet);
         m_inputList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      shared::CDataContainer pulseContainerName = details.get<shared::CDataContainer>("Pulses");
      for (int counter = 0; counter < WES_PULSE_QTY; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CPulse> temp = boost::make_shared<equipments::subdevices::CPulse>(api,
                                                                                                                     keywordsToDeclare,
                                                                                                                     m_deviceName,
                                                                                                                     pulseContainerName.get<std::string>("P" + boost::lexical_cast<std::string>(counter)),
                                                                                                                     pulseContainerName.get<std::string>("P" + boost::lexical_cast<std::string>(counter)));
         m_PulseList.push_back(temp);
      }

      shared::CDataContainer clampContainerName = details.get<shared::CDataContainer>("Clamps");
      for (int counter = 0; counter < WES_CLAMP_QTY; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CClamp> temp = boost::make_shared<equipments::subdevices::CClamp>(api,
                                                                                                                     keywordsToDeclare,
                                                                                                                     m_configuration.isInstantCurrentClampRegistered(counter),
                                                                                                                     m_deviceName,
                                                                                                                     clampContainerName.get<std::string>("C" + boost::lexical_cast<std::string>(counter)));
         m_ClampList.push_back(temp);
      }

      shared::CDataContainer analogContainerName = details.get<shared::CDataContainer>("Analogs");
      for (int counter = 0; counter < WES_ANA_QTY; ++counter)
      {
         boost::shared_ptr<specificHistorizers::CAnalog> temp = boost::make_shared<specificHistorizers::CAnalog>(analogContainerName.get<std::string>("A" + boost::lexical_cast<std::string>(counter)),
                                                                                                                  yApi::EKeywordAccessMode::kGet);
         m_AnalogList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      YADOMS_LOG(information) << "Load configuration for " << m_deviceName;
   }

   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const shared::CDataContainer& deviceConfiguration,
                                const boost::shared_ptr<IWESConfiguration> pluginConfiguration
   ):
      m_deviceName(device),
      m_deviceType("WES"),
      m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("Status"))
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      std::string relayName[2], TICName[2], ClampName[4], AnalogName[4], inputName[2];
      std::string contract[2];
      std::string PulseType[4];
      std::string PulseName[4];

      m_deviceStatus->set(specificHistorizers::EdeviceStatus::kUndefined);
      keywordsToDeclare.push_back(m_deviceStatus);

      try {
         m_configuration.initializeWith(deviceConfiguration);
         deviceConfiguration.printToLog(YADOMS_LOG(information));

         if (pluginConfiguration->isRetrieveNamesFromdevice())
         {
            shared::CDataContainer credentials;

            credentials.set("user", m_configuration.getUser());
            credentials.set("password", m_configuration.getPassword());

            credentials.printToLog(YADOMS_LOG(information));

            // request to obtain names
            std::string CGXfileName = "WESNAMES.CGX";
            shared::CDataContainer results = urlManager::readFileState(m_configuration.getIPAddressWithSocket(),
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
         for (int counter = 0; counter < WES_RELAY_QTY; ++counter)
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
         for (int counter = 0; counter < WES_TIC_QTY; ++counter)
         {
            boost::shared_ptr<equipments::subdevices::CTIC> temp = boost::make_shared<equipments::subdevices::CTIC>(api, m_deviceName + " - " + TICName[counter], contract[counter]);
            m_TICList.push_back(temp);
         }

         // Pulse Counters Configuration
         for (int counter = 0; counter < WES_PULSE_QTY; ++counter)
         {
            boost::shared_ptr<equipments::subdevices::CPulse> temp = boost::make_shared<equipments::subdevices::CPulse>(api,
                                                                                                                        keywordsToDeclare,
                                                                                                                        m_deviceName,
                                                                                                                        PulseName[counter],
                                                                                                                        PulseType[counter]);
            m_PulseList.push_back(temp);
         }

         // Current clamp Configuration
         for (int counter = 0; counter < WES_CLAMP_QTY; ++counter)
         {
            boost::shared_ptr<equipments::subdevices::CClamp> temp = boost::make_shared<equipments::subdevices::CClamp>(api,
                                                                                                                        keywordsToDeclare,
                                                                                                                        m_configuration.isInstantCurrentClampRegistered(counter),
                                                                                                                        m_deviceName,
                                                                                                                        ClampName[counter]);
            m_ClampList.push_back(temp);
         }

         // Analog Values Configuration
         for (int counter = 0; counter < WES_ANA_QTY; ++counter)
         {
            boost::shared_ptr<specificHistorizers::CAnalog> temp = boost::make_shared<specificHistorizers::CAnalog>(AnalogName[counter],
                                                                                                                    yApi::EKeywordAccessMode::kGet);
            m_AnalogList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // Save names into details
         shared::CDataContainer details;
         details.set("provider", "WES");
         details.set("type", m_deviceType);

         shared::CDataContainer TICContainerName;
         for (int counter = 0; counter < WES_TIC_QTY; ++counter)
         {
            TICContainerName.set("TIC" + boost::lexical_cast<std::string>(counter), m_TICList[counter]->name());
            TICContainerName.set("contract" + boost::lexical_cast<std::string>(counter), contract[counter]);
         }
         details.set("TIC", TICContainerName);

         shared::CDataContainer RelayContainerName;
         for (int counter = 0; counter < WES_RELAY_QTY; ++counter)
         {
            RelayContainerName.set("R" + boost::lexical_cast<std::string>(counter), m_relaysList[counter]->getKeyword());
         }
         details.set("Relays", RelayContainerName);

         shared::CDataContainer inputContainerName;
         for (int counter = 0; counter < WES_INPUT_QTY; ++counter)
         {
            inputContainerName.set("I" + boost::lexical_cast<std::string>(counter), m_inputList[counter]->getKeyword());
         }
         details.set("Inputs", inputContainerName);

         shared::CDataContainer pulseContainerName;
         for (int counter = 0; counter < WES_PULSE_QTY; ++counter)
         {
            pulseContainerName.set("P" + boost::lexical_cast<std::string>(counter), m_PulseList[counter]->name());
            pulseContainerName.set("T" + boost::lexical_cast<std::string>(counter), PulseType[counter]);
         }
         details.set("Pulses", pulseContainerName);

         shared::CDataContainer clampContainerName;
         for (int counter = 0; counter < WES_CLAMP_QTY; ++counter)
         {
            clampContainerName.set("C" + boost::lexical_cast<std::string>(counter), m_ClampList[counter]->name());
         }
         details.set("Clamps", clampContainerName);

         shared::CDataContainer analogContainerName;
         for (int counter = 0; counter < WES_ANA_QTY; ++counter)
         {
            analogContainerName.set("A" + boost::lexical_cast<std::string>(counter), m_AnalogList[counter]->getKeyword());
         }
         details.set("Analogs", analogContainerName);

         details.printToLog(YADOMS_LOG(information));

         std::string model = "WES";

         //Déclaration of all IOs
         api->declareDevice(device, m_deviceType, model, keywordsToDeclare, details);
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << e.what();
      }
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
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize = { m_deviceStatus };
      std::string CGXfileName = "WESVALUES.CGX";

      try {
         shared::CDataContainer credentials;
         credentials.set("user", m_configuration.getUser());
         credentials.set("password", m_configuration.getPassword());

         shared::CDataContainer results = urlManager::readFileState(m_configuration.getIPAddressWithSocket(),
                                                                    credentials,
                                                                    CGXfileName);

         // Reading relays - historize only on change value or when the historization is forced (initialization, for example)      
         try {
            updateSwitchValue(keywordsToHistorize, m_relaysList[0], results.get<bool>("RL1"), forceHistorization);
            updateSwitchValue(keywordsToHistorize, m_relaysList[1], results.get<bool>("RL2"), forceHistorization);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(warning) << e.what();
         }

         try {
            updateSwitchValue(keywordsToHistorize, m_inputList[0], results.get<bool>("I1"), forceHistorization);
            updateSwitchValue(keywordsToHistorize, m_inputList[1], results.get<bool>("I2"), forceHistorization);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(warning) << e.what();
         }

         //Reading clamp values
         for (int counter = 0; counter < WES_CLAMP_QTY; ++counter)
         {
            try {
               m_ClampList[counter]->updateFromDevice(api,
                                                      keywordsToHistorize,
                                                      m_configuration.isInstantCurrentClampRegistered(counter),
                                                      results.get<double>("IPC" + boost::lexical_cast<std::string>(counter + 1) + "_val"),
                                                      results.get<Poco::Int64>("WHPC" + boost::lexical_cast<std::string>(counter + 1) + "_val"));
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << e.what();
            }
         }

         //Reading pulse values
         for (int counter = 0; counter < WES_PULSE_QTY; ++counter)
         {
            try {
               m_PulseList[counter]->updateFromDevice(api,
                                                      keywordsToHistorize,
                                                      results.get<std::string>("PLSU" + boost::lexical_cast<std::string>(counter + 1)),
                                                      results.get<Poco::Int64>("debit" + boost::lexical_cast<std::string>(counter + 1) + "_val"),
                                                      results.get<std::string>("consJ" + boost::lexical_cast<std::string>(counter + 1) + "_val"));
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << e.what();
            }
         }

         //Reading analog values
         for (int counter = 0; counter < WES_ANA_QTY; ++counter)
         {
            try {
               m_AnalogList[counter]->set(results.get<unsigned int>("ad" + boost::lexical_cast<std::string>(counter + 1)));
               keywordsToHistorize.push_back(m_AnalogList[counter]);
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << e.what();
            }
         }

         api->historizeData(m_deviceName, keywordsToHistorize);

         // TIC Counters Values -> independant from the others keywords
         for (int counter = 0; counter < WES_TIC_QTY; ++counter)
         {
            m_TICList[counter]->updateFromDevice(api,
                                                 m_deviceStatus->get(),
                                                 results.get<std::string>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_abo_name"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_1"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_2"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_3"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_4"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_5"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_6"));
         }

         setDeviceState(specificHistorizers::EdeviceStatus::kOk);
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << e.what();
         setDeviceState(specificHistorizers::EdeviceStatus::kError);
         api->historizeData(m_deviceName, keywordsToHistorize);
      }
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
                                           const shared::CDataContainer& newConfiguration)
   {
      m_configuration.initializeWith(newConfiguration);
      api->updateDeviceConfiguration(m_deviceName, newConfiguration);
      YADOMS_LOG(information) << "Configuration updated for " << m_deviceName;
   }

   void CWESEquipment::sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                   std::string& keyword,
                                   std::string& command)
   {
      shared::CDataContainer credentials;
      shared::CDataContainer parameters;

      try {
         credentials.set("user", m_configuration.getUser());
         credentials.set("password", m_configuration.getPassword());

         //parameters to be filled

         shared::CDataContainer results = urlManager::setRelayState(m_configuration.getIPAddressWithSocket(),
                                                                    credentials,
                                                                    parameters);
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << e.what();
      }
   }

   void CWESEquipment::setDeviceState(specificHistorizers::EdeviceStatus newState)
   {
      if (m_deviceStatus->get() != newState)
      {
         switch (newState)
         {
         case specificHistorizers::EdeviceStatus::kOkValue:
            m_deviceStatus->set(specificHistorizers::EdeviceStatus::kOk);
            break;
         case specificHistorizers::EdeviceStatus::kErrorValue:
            m_deviceStatus->set(specificHistorizers::EdeviceStatus::kError);
            break;
         case specificHistorizers::EdeviceStatus::kTimeOutValue:
            m_deviceStatus->set(specificHistorizers::EdeviceStatus::kTimeOut);
            break;
         case specificHistorizers::EdeviceStatus::kUndefinedValue:
            m_deviceStatus->set(specificHistorizers::EdeviceStatus::kUndefined);
            break;
         default:
            YADOMS_LOG(error) << "this plugin status does not exist : " << newState;
            break;
         }

         m_deviceStatus->set(newState);
      }
   }

   void CWESEquipment::remove(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Delete 2 others devices
      for (int counter = 0; counter < WES_TIC_QTY; ++counter)
         m_TICList[counter]->remove(api);
   }

   /*
   tu peux controler les relais en UDP, TCP ou par des requêtes HTTP:
   Par une requête HTTP :
   http://WES/RL.cgi?rl1=ON&rl2=OFF
   http://192.168.1.37/RL.cgx?rl9=ON -> Les 2 relais
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