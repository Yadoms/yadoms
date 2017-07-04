#include "stdafx.h"
#include "WESEquipment.h"
#include <shared/DataContainer.h>
#include "../urlManager.h"
#include <shared/Log.h>
#include "../http/timeOutException.hpp"

namespace equipments
{
   const CWESEquipment::WESIOMapping CWESEquipment::WESv1 = {2, 2, 2, 2, 2, 4};
   const CWESEquipment::WESIOMapping CWESEquipment::WESv2 = {2, 2, 2, 4, 4, 4};

   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const shared::CDataContainer& deviceConfiguration)
      : m_deviceName(device),
        m_deviceType("WES"),
        m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("Status")),
        m_version(0)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToDeclare;

      keywordsToDeclare.push_back(m_deviceStatus);
      m_configuration.initializeWith(deviceConfiguration);
      auto details = api->getDeviceDetails(device);

      // get the revision, for E/S numbers
      m_version = details.get<int>("version");

      if (m_version == 1)
         m_WESIOMapping = WESv1;
      else if (m_version == 2)
         m_WESIOMapping = WESv2;
      else
         throw shared::exception::CException("WES Revision not properly set : " + boost::lexical_cast<std::string>(m_version));

      auto TICContainerName = details.get<shared::CDataContainer>("TIC");
      for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
      {
         auto temp = boost::make_shared<subdevices::CTIC>(api,
                                                          TICContainerName.get<std::string>("TIC" + boost::lexical_cast<std::string>(counter)),
                                                          TICContainerName.get<std::string>("CounterId" + boost::lexical_cast<std::string>(counter)),
                                                          TICContainerName.get<subdevices::ContractAvailable>("contract" + boost::lexical_cast<std::string>(counter)));
         m_TICList.push_back(temp);
      }

      auto RelayContainerName = details.get<shared::CDataContainer>("Relays");
      for (int counter = 0; counter < m_WESIOMapping.relayQty; ++counter)
      {
         auto temp = boost::make_shared<yApi::historization::CSwitch>(RelayContainerName.get<std::string>("R" + boost::lexical_cast<std::string>(counter)),
                                                                      yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      auto inputContainerName = details.get<shared::CDataContainer>("Inputs");
      for (int counter = 0; counter < m_WESIOMapping.inputQty; ++counter)
      {
         auto temp = boost::make_shared<yApi::historization::CSwitch>(inputContainerName.get<std::string>("I" + boost::lexical_cast<std::string>(counter)),
                                                                      yApi::EKeywordAccessMode::kGet);
         m_inputList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      auto pulseContainerName = details.get<shared::CDataContainer>("Pulses");
      for (int counter = 0; counter < m_WESIOMapping.pulseQty; ++counter)
      {
         auto temp = boost::make_shared<subdevices::CPulse>(api,
                                                            keywordsToDeclare,
                                                            m_deviceName,
                                                            pulseContainerName.get<std::string>("P" + boost::lexical_cast<std::string>(counter)),
                                                            pulseContainerName.get<subdevices::EUnit>("T" + boost::lexical_cast<std::string>(counter)));
         m_PulseList.push_back(temp);
      }

      auto clampContainerName = details.get<shared::CDataContainer>("Clamps");
      for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
      {
         auto temp = boost::make_shared<subdevices::CClamp>(api,
                                                            keywordsToDeclare,
                                                            m_deviceName,
                                                            clampContainerName.get<std::string>("C" + boost::lexical_cast<std::string>(counter)));
         m_ClampList.push_back(temp);
      }

      auto analogContainerName = details.get<shared::CDataContainer>("Analogs");
      for (int counter = 0; counter < m_WESIOMapping.anaQty; ++counter)
      {
         auto temp = boost::make_shared<specificHistorizers::CAnalog>(analogContainerName.get<std::string>("A" + boost::lexical_cast<std::string>(counter)),
                                                                      yApi::EKeywordAccessMode::kGet);
         m_AnalogList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      YADOMS_LOG(information) << "Load configuration for " << m_deviceName;
   }

   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const shared::CDataContainer& deviceConfiguration,
                                const boost::shared_ptr<IWESConfiguration> pluginConfiguration)
      : m_deviceName(device),
        m_deviceType("WES"),
        m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("Status")),
        m_version(0)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToDeclare;
      std::string relayName[2], TICName[2], ClampName[4], AnalogName[4], inputName[2];
      subdevices::ContractAvailable contract[2];
      subdevices::EUnit PulseType[4];
      std::string PulseName[4];
      std::string counterId[2];

      try
      {
         keywordsToDeclare.push_back(m_deviceStatus);
         m_configuration.initializeWith(deviceConfiguration);
         deviceConfiguration.printToLog(YADOMS_LOG(information));

         shared::CDataContainer credentials;

         credentials.set("user", m_configuration.getUser());
         credentials.set("password", m_configuration.getPassword());

         // request to obtain the WES revision
         std::string CGXfileName = "WESVERSION.CGX";
         auto results = urlManager::readFileState(m_configuration.getIPAddressWithSocket(),
                                                  credentials,
                                                  CGXfileName);

         // get the revision, for E/S numbers
         m_version = results.get<int>("version");

         if (m_version == 1)
            m_WESIOMapping = WESv1;
         else if (m_version == 2)
            m_WESIOMapping = WESv2;
         else
            throw shared::exception::CException("WES Revision not properly set : " + boost::lexical_cast<std::string>(m_version));

         if (pluginConfiguration->isRetrieveNamesFromdevice())
         {
            credentials.printToLog(YADOMS_LOG(information));

            // request to obtain names
            CGXfileName = "WESNAMES" + boost::lexical_cast<std::string>(m_version) + ".CGX";
            results = urlManager::readFileState(m_configuration.getIPAddressWithSocket(),
                                                credentials,
                                                CGXfileName);

            contract[0] = results.get<subdevices::ContractAvailable>("CPT1_abo_name");
            contract[1] = results.get<subdevices::ContractAvailable>("CPT2_abo_name");

            counterId[0] = results.get<std::string>("CPT1_adco");
            counterId[1] = results.get<std::string>("CPT2_adco");

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

            PulseType[0] = results.get<subdevices::EUnit>("PLSU1");
            PulseType[1] = results.get<subdevices::EUnit>("PLSU2");
            PulseType[2] = results.get<subdevices::EUnit>("PLSU3");
            PulseType[3] = results.get<subdevices::EUnit>("PLSU4");
         }
         else // Defaults names
         {
            contract[0] = subdevices::NotAvailable;
            contract[1] = subdevices::NotAvailable;
            counterId[0] = "000000000000";
            counterId[1] = "000000000000";

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

            PulseType[0] = subdevices::EUnit::undefined;
            PulseType[1] = subdevices::EUnit::undefined;
            PulseType[2] = subdevices::EUnit::undefined;
            PulseType[3] = subdevices::EUnit::undefined;
         }

         // Relay Configuration
         for (int counter = 0; counter < m_WESIOMapping.relayQty; ++counter)
         {
            auto temp = boost::make_shared<yApi::historization::CSwitch>(relayName[counter],
                                                                         yApi::EKeywordAccessMode::kGetSet);
            m_relaysList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // Input Configuration
         for (int counter = 0; counter < m_WESIOMapping.inputQty; ++counter)
         {
            auto temp = boost::make_shared<yApi::historization::CSwitch>(inputName[counter],
                                                                         yApi::EKeywordAccessMode::kGet);
            m_inputList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // TIC Counters Configuration
         for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
         {
            auto temp = boost::make_shared<subdevices::CTIC>(api,
                                                             m_deviceName + " - " + TICName[counter],
                                                             counterId[counter],
                                                             contract[counter]);
            m_TICList.push_back(temp);
         }

         // Pulse Counters Configuration
         for (int counter = 0; counter < m_WESIOMapping.pulseQty; ++counter)
         {
            auto temp = boost::make_shared<subdevices::CPulse>(api,
                                                               keywordsToDeclare,
                                                               m_deviceName,
                                                               PulseName[counter],
                                                               PulseType[counter]);
            m_PulseList.push_back(temp);
         }

         // Current clamp Configuration
         for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
         {
            auto temp = boost::make_shared<subdevices::CClamp>(api,
                                                               keywordsToDeclare,
                                                               m_deviceName,
                                                               ClampName[counter]);
            m_ClampList.push_back(temp);
         }

         // Analog Values Configuration
         for (int counter = 0; counter < m_WESIOMapping.anaQty; ++counter)
         {
            auto temp = boost::make_shared<specificHistorizers::CAnalog>(AnalogName[counter],
                                                                         yApi::EKeywordAccessMode::kGet);
            m_AnalogList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // Save names into details
         shared::CDataContainer details;
         details.set("provider", "WES");
         details.set("type", m_deviceType);

         shared::CDataContainer TICContainerName;
         for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
         {
            TICContainerName.set("TIC" + boost::lexical_cast<std::string>(counter), m_TICList[counter]->name());
            TICContainerName.set("contract" + boost::lexical_cast<std::string>(counter), contract[counter]);
            TICContainerName.set("CounterId" + boost::lexical_cast<std::string>(counter), counterId[counter]);
         }
         details.set("TIC", TICContainerName);

         shared::CDataContainer RelayContainerName;
         for (int counter = 0; counter < m_WESIOMapping.relayQty; ++counter)
         {
            RelayContainerName.set("R" + boost::lexical_cast<std::string>(counter), m_relaysList[counter]->getKeyword());
         }
         details.set("Relays", RelayContainerName);

         shared::CDataContainer inputContainerName;
         for (int counter = 0; counter < m_WESIOMapping.inputQty; ++counter)
         {
            inputContainerName.set("I" + boost::lexical_cast<std::string>(counter), m_inputList[counter]->getKeyword());
         }
         details.set("Inputs", inputContainerName);

         shared::CDataContainer pulseContainerName;
         for (int counter = 0; counter < m_WESIOMapping.pulseQty; ++counter)
         {
            pulseContainerName.set("P" + boost::lexical_cast<std::string>(counter), m_PulseList[counter]->name());
            pulseContainerName.set("T" + boost::lexical_cast<std::string>(counter), PulseType[counter]);
         }
         details.set("Pulses", pulseContainerName);

         shared::CDataContainer clampContainerName;
         for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
         {
            clampContainerName.set("C" + boost::lexical_cast<std::string>(counter), m_ClampList[counter]->name());
         }
         details.set("Clamps", clampContainerName);

         shared::CDataContainer analogContainerName;
         for (int counter = 0; counter < m_WESIOMapping.anaQty; ++counter)
         {
            analogContainerName.set("A" + boost::lexical_cast<std::string>(counter), m_AnalogList[counter]->getKeyword());
         }
         details.set("Analogs", analogContainerName);
         details.set("version", m_version);

         details.printToLog(YADOMS_LOG(information));

         //Déclaration of all IOs

         auto existingModel = api->getDeviceModel(m_deviceName);
         if (existingModel.empty())
            api->updateDeviceModel(m_deviceName, "WES");

         api->updateDeviceDetails(m_deviceName, details);

         api->declareKeywords(m_deviceName, keywordsToDeclare);
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

   std::string CWESEquipment::getDeviceType() const
   {
      return m_deviceType;
   }

   void CWESEquipment::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                                        bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize;

      try
      {
         if (m_version == 0 || m_version > 2)
            throw shared::exception::CException("m_version is not set properly : " + boost::lexical_cast<std::string>(m_version));

         std::string CGXfileName = "WESVALUES" + boost::lexical_cast<std::string>(m_version) + ".CGX";

         shared::CDataContainer credentials;
         credentials.set("user", m_configuration.getUser());
         credentials.set("password", m_configuration.getPassword());

         shared::CDataContainer results = urlManager::readFileState(m_configuration.getIPAddressWithSocket(),
                                                                    credentials,
                                                                    CGXfileName);

         results.printToLog(YADOMS_LOG(trace));

         // Reading relays - historize only on change value or when the historization is forced (initialization, for example)      
         try
         {
            updateSwitchValue(keywordsToHistorize, m_relaysList[0], results.get<bool>("RL1"), forceHistorization);
            updateSwitchValue(keywordsToHistorize, m_relaysList[1], results.get<bool>("RL2"), forceHistorization);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(warning) << "Exception reading relays" << e.what();
         }

         try
         {
            updateSwitchValue(keywordsToHistorize, m_inputList[0], results.get<bool>("I1"), forceHistorization);
            updateSwitchValue(keywordsToHistorize, m_inputList[1], results.get<bool>("I2"), forceHistorization);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(warning) << "Exception reading inputs" << e.what();
         }

         //Reading clamp values
         for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
         {
            try
            {
               m_ClampList[counter]->updateFromDevice(api,
                                                      keywordsToHistorize,
                                                      results.get<Poco::Int64>("WHPC" + boost::lexical_cast<std::string>(counter + 1) + "_val"));
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << "Error reading clamp " << "WHPC" + boost::lexical_cast<std::string>(counter + 1) + "_val" << e.what();
            }
         }

         //Reading pulse values
         for (int counter = 0; counter < m_WESIOMapping.pulseQty; ++counter)
         {
            try
            {
               m_PulseList[counter]->updateFromDevice(api,
                                                      keywordsToHistorize,
                                                      results.get<subdevices::EUnit>("PLSU" + boost::lexical_cast<std::string>(counter + 1)),
                                                      results.get<double>("actuel" + boost::lexical_cast<std::string>(counter + 1) + "_val"));
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << "Error reading pulse " << "actuel" + boost::lexical_cast<std::string>(counter + 1) + "_val :" << e.what();
            }
         }

         //Reading analog values
         for (int counter = 0; counter < m_WESIOMapping.anaQty; ++counter)
         {
            try
            {
               m_AnalogList[counter]->set(results.get<unsigned int>("ad" + boost::lexical_cast<std::string>(counter + 1)));
               keywordsToHistorize.push_back(m_AnalogList[counter]);
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << "Exception reading analog " << "ad" + boost::lexical_cast<std::string>(counter + 1) << e.what();
            }
         }

         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kOk);
         api->historizeData(m_deviceName, keywordsToHistorize);

         // TIC Counters Values -> independant from the others keywords
         for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
         {
            m_TICList[counter]->updateFromDevice(api,
                                                 m_deviceStatus->get(),
                                                 results.get<subdevices::ContractAvailable>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_abo_name"),
                                                 results.get<std::string>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_adco"),
                                                 results.get<int>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_PTarif"),
                                                 results.get<unsigned int>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_P"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_1"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_2"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_3"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_4"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_5"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_6"),
                                                 results.get<int>("DEMAIN_" + boost::lexical_cast<std::string>(counter + 1)));
         }
      }
      catch (CTimeOutException&)
      {
         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kTimeOut);
         api->historizeData(m_deviceName, keywordsToHistorize);

         for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
            m_TICList[counter]->setDeviceState(api, specificHistorizers::EWESdeviceStatus::kTimeOut);
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << e.what();
         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kError);
         api->historizeData(m_deviceName, keywordsToHistorize);

         if (m_TICList.size() == m_WESIOMapping.ticQty)
         {
            for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
               m_TICList[counter]->setDeviceState(api, specificHistorizers::EWESdeviceStatus::kError);
         }
      }
   }

   void CWESEquipment::updateSwitchValue(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywordsToHistorize,
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
                                   const std::string& keyword,
                                   boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize;

      try
      {
         shared::CDataContainer credentials;
         shared::CDataContainer parameters;
         bool newValue = false;
         std::string stringState;
         int counter = 0;

         credentials.set("user", m_configuration.getUser());
         credentials.set("password", m_configuration.getPassword());

         newValue = boost::lexical_cast<bool>(command->getBody());
         if (newValue)
            stringState = "ON";
         else
            stringState = "OFF";

         std::vector<boost::shared_ptr<yApi::historization::CSwitch>>::const_iterator iteratorRelay;
         for (iteratorRelay = m_relaysList.begin(); (iteratorRelay != m_relaysList.end() && (command->getKeyword() != (*iteratorRelay)->getKeyword())); ++iteratorRelay)
            ++counter;

         parameters.set("rl" + boost::lexical_cast<std::string>(counter + 1), stringState);

         if (iteratorRelay == m_relaysList.end())
            throw shared::exception::CException("Failed to identify the relay");

         shared::CDataContainer results = urlManager::setRelayState(m_configuration.getIPAddressWithSocket(),
                                                                    credentials,
                                                                    parameters);

         // For security, we check if the results contain the value, with the corresponding new state
         if (results.containsChild("Relai1"))
         {
            if (results.get<std::string>("Relai1") == "ON" && !m_relaysList[0]->get())
            {
               m_relaysList[0]->set(true);
               keywordsToHistorize.push_back(m_relaysList[0]);
            }
            else if (results.get<std::string>("Relai1") == "OFF" && m_relaysList[0]->get())
            {
               m_relaysList[0]->set(false);
               keywordsToHistorize.push_back(m_relaysList[0]);
            }
         }

         if (results.containsChild("Relai2"))
         {
            if (results.get<std::string>("Relai2") == "ON" && !m_relaysList[1]->get())
            {
               m_relaysList[1]->set(true);
               keywordsToHistorize.push_back(m_relaysList[1]);
            }
            else if (results.get<std::string>("Relai2") == "OFF" && m_relaysList[1]->get())
            {
               m_relaysList[1]->set(false);
               keywordsToHistorize.push_back(m_relaysList[1]);
            }
         }

         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kOk);
         api->historizeData(m_deviceName, keywordsToHistorize);
      }
      catch (std::exception& e)
      {
         keywordsToHistorize.clear();
         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kError);
         api->historizeData(m_deviceName, keywordsToHistorize);
         YADOMS_LOG(error) << e.what();
      }
   }

   void CWESEquipment::setDeviceState(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywordsToHistorize,
                                      specificHistorizers::EWESdeviceStatus newState)
   {
      if (m_deviceStatus->get() != newState)
      {
         m_deviceStatus->set(newState);
         keywordsToHistorize.push_back(m_deviceStatus);
         YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
      }
   }

   specificHistorizers::EWESdeviceStatus CWESEquipment::getStatus() const
   {
      return m_deviceStatus->get();
   }

   void CWESEquipment::remove(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Delete 2 others devices
      for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
         m_TICList[counter]->remove(api);
   }

   CWESEquipment::~CWESEquipment()
   {
   }
}// namespace equipments


