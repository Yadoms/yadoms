#include "stdafx.h"
#include "WESEquipment.h"
#include <shared/DataContainer.h>
#include "noInformationException.hpp"
#include "../urlManager.h"
#include <shared/Log.h>
#include "../http/timeOutException.hpp"

namespace equipments
{
   const CWESEquipment::WESIOMapping CWESEquipment::WESv1 = { 2, 2, 2, 4, 4, 4 };
   const CWESEquipment::WESIOMapping CWESEquipment::WESv2 = { 2, 2, 2, 2, 2, 4 };

   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const shared::CDataContainer& deviceConfiguration) :
      m_deviceName(device),
      m_deviceType("WES"),
      m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("Status"))
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

      m_deviceStatus->set(specificHistorizers::EWESdeviceStatus::kUndefined);
      keywordsToDeclare.push_back(m_deviceStatus);

      m_configuration.initializeWith(deviceConfiguration);
      shared::CDataContainer details = api->getDeviceDetails(device);

      // get the revision, for E/S numbers
      int version = details.get<int>("version");

      if (version == 1)
         m_WESIOMapping = WESv1;
      else if (version == 2)
         m_WESIOMapping = WESv2;
      else
         throw shared::exception::CException("WES Revision not properly set : " + boost::lexical_cast<std::string>(version));

      shared::CDataContainer TICContainerName = details.get<shared::CDataContainer>("TIC");
      for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CTIC> temp = boost::make_shared<equipments::subdevices::CTIC>(api, 
                                                                                                                 TICContainerName.get<std::string>("TIC" + boost::lexical_cast<std::string>(counter)),
                                                                                                                 TICContainerName.get<std::string>("CounterId" + boost::lexical_cast<std::string>(counter)),
                                                                                                                 TICContainerName.get<equipments::subdevices::ContractAvailable>("contract" + boost::lexical_cast<std::string>(counter)));
         m_TICList.push_back(temp);
      }

      shared::CDataContainer RelayContainerName = details.get<shared::CDataContainer>("Relays");
      for (int counter = 0; counter < m_WESIOMapping.relayQty; ++counter)
      {
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(RelayContainerName.get<std::string>("R" + boost::lexical_cast<std::string>(counter)),
                                                                                                                  yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      shared::CDataContainer inputContainerName = details.get<shared::CDataContainer>("Inputs");
      for (int counter = 0; counter < m_WESIOMapping.inputQty; ++counter)
      {
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(inputContainerName.get<std::string>("I" + boost::lexical_cast<std::string>(counter)),
                                                                                                                  yApi::EKeywordAccessMode::kGet);
         m_inputList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      shared::CDataContainer pulseContainerName = details.get<shared::CDataContainer>("Pulses");
      for (int counter = 0; counter < m_WESIOMapping.pulseQty; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CPulse> temp = boost::make_shared<equipments::subdevices::CPulse>(api,
                                                                                                                     keywordsToDeclare,
                                                                                                                     m_deviceName,
                                                                                                                     pulseContainerName.get<std::string>("P" + boost::lexical_cast<std::string>(counter)),
                                                                                                                     pulseContainerName.get<std::string>("P" + boost::lexical_cast<std::string>(counter)));
         m_PulseList.push_back(temp);
      }

      shared::CDataContainer clampContainerName = details.get<shared::CDataContainer>("Clamps");
      for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
      {
         boost::shared_ptr<equipments::subdevices::CClamp> temp = boost::make_shared<equipments::subdevices::CClamp>(api,
                                                                                                                     keywordsToDeclare,
                                                                                                                     m_configuration.isInstantCurrentClampRegistered(counter),
                                                                                                                     m_deviceName,
                                                                                                                     clampContainerName.get<std::string>("C" + boost::lexical_cast<std::string>(counter)));
         m_ClampList.push_back(temp);
      }

      shared::CDataContainer analogContainerName = details.get<shared::CDataContainer>("Analogs");
      for (int counter = 0; counter < m_WESIOMapping.anaQty; ++counter)
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
      equipments::subdevices::ContractAvailable contract[2];
      std::string PulseType[4];
      std::string PulseName[4];
      std::string counterId[2];

      m_deviceStatus->set(specificHistorizers::EWESdeviceStatus::kUndefined);
      keywordsToDeclare.push_back(m_deviceStatus);

      try {
         m_configuration.initializeWith(deviceConfiguration);
         deviceConfiguration.printToLog(YADOMS_LOG(information));

         shared::CDataContainer credentials;

         credentials.set("user", m_configuration.getUser());
         credentials.set("password", m_configuration.getPassword());

         // request to obtain the WES revision
         std::string CGXfileName = "WESVERSION.CGX";
         shared::CDataContainer results = urlManager::readFileState(m_configuration.getIPAddressWithSocket(),
                                                                    credentials,
                                                                    CGXfileName);

         // get the revision, for E/S numbers
         int version = results.get<int>("version");

         if (version == 1)
            m_WESIOMapping = WESv1;
         else if (version == 2)
            m_WESIOMapping = WESv2;
         else
            throw shared::exception::CException("WES Revision not properly set : " + boost::lexical_cast<std::string>(version));

         if (pluginConfiguration->isRetrieveNamesFromdevice())
         {
            credentials.printToLog(YADOMS_LOG(information));

            // request to obtain names
            CGXfileName = "WESNAMES.CGX";
            results = urlManager::readFileState(m_configuration.getIPAddressWithSocket(),
                                                credentials,
                                                CGXfileName);

            contract[0] = results.get<equipments::subdevices::ContractAvailable>("CPT1_abo_name");
            contract[1] = results.get<equipments::subdevices::ContractAvailable>("CPT2_abo_name");

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

            PulseType[0] = results.get<std::string>("PLSU1");
            PulseType[1] = results.get<std::string>("PLSU2");
            PulseType[2] = results.get<std::string>("PLSU3");
            PulseType[3] = results.get<std::string>("PLSU4");
         }
         else  // Defaults names
         {
            contract[0] = equipments::subdevices::NotAvailable;
            contract[1] = equipments::subdevices::NotAvailable;
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

            PulseType[0] = "ND";
            PulseType[1] = "ND";
            PulseType[2] = "ND";
            PulseType[3] = "ND";
         }

         // Relay Configuration
         for (int counter = 0; counter < m_WESIOMapping.relayQty; ++counter)
         {
            boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(relayName[counter],
                                                                                                                    yApi::EKeywordAccessMode::kGetSet);
            m_relaysList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // Input Configuration
         for (int counter = 0; counter < m_WESIOMapping.inputQty; ++counter)
         {
            boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(inputName[counter],
                                                                                                                    yApi::EKeywordAccessMode::kGet);
            m_inputList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // TIC Counters Configuration
         for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
         {
            boost::shared_ptr<equipments::subdevices::CTIC> temp = boost::make_shared<equipments::subdevices::CTIC>(api, 
                                                                                                                    m_deviceName + " - " + TICName[counter], 
                                                                                                                    counterId[counter],
                                                                                                                    contract[counter]);
            m_TICList.push_back(temp);
         }

         // Pulse Counters Configuration
         for (int counter = 0; counter < m_WESIOMapping.pulseQty; ++counter)
         {
            boost::shared_ptr<equipments::subdevices::CPulse> temp = boost::make_shared<equipments::subdevices::CPulse>(api,
                                                                                                                        keywordsToDeclare,
                                                                                                                        m_deviceName,
                                                                                                                        PulseName[counter],
                                                                                                                        PulseType[counter]);
            m_PulseList.push_back(temp);
         }

         // Current clamp Configuration
         for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
         {
            boost::shared_ptr<equipments::subdevices::CClamp> temp = boost::make_shared<equipments::subdevices::CClamp>(api,
                                                                                                                        keywordsToDeclare,
                                                                                                                        m_configuration.isInstantCurrentClampRegistered(counter),
                                                                                                                        m_deviceName,
                                                                                                                        ClampName[counter]);
            m_ClampList.push_back(temp);
         }

         // Analog Values Configuration
         for (int counter = 0; counter < m_WESIOMapping.anaQty; ++counter)
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

         details.set("version", version);

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
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;
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
         for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
         {
            try {
               m_ClampList[counter]->updateFromDevice(api,
                                                      keywordsToHistorize,
                                                      results.get<double>("IPC" + boost::lexical_cast<std::string>(counter + 1) + "_val"),
                                                      results.get<Poco::Int64>("WHPC" + boost::lexical_cast<std::string>(counter + 1) + "_val"));
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(warning) << e.what();
            }
         }

         //Reading pulse values
         for (int counter = 0; counter < m_WESIOMapping.pulseQty; ++counter)
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
         for (int counter = 0; counter < m_WESIOMapping.anaQty; ++counter)
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

         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kOk);
         api->historizeData(m_deviceName, keywordsToHistorize);

         // TIC Counters Values -> independant from the others keywords
         for (int counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
         {
            m_TICList[counter]->updateFromDevice(api,
                                                 m_deviceStatus->get(),
                                                 results.get<equipments::subdevices::ContractAvailable>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_abo_name"),
                                                 results.get<std::string>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_adco"),
                                                 results.get<int>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_PTarif"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_P"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_1"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_2"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_3"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_4"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_5"),
                                                 results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_6"));
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

      for (int counter = 0; counter < m_WESIOMapping.clampQty; ++counter)
      {
         // update the clamp configuration
         m_ClampList[counter]->updateConfiguration(api, m_configuration.isInstantCurrentClampRegistered(counter));
      }

      YADOMS_LOG(information) << "Configuration updated for " << m_deviceName;
   }

   void CWESEquipment::sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const std::string& keyword,
                                   boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;

      try {
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

         std::vector<boost::shared_ptr<yApi::historization::CSwitch> >::const_iterator iteratorRelay;
         for (iteratorRelay = m_relaysList.begin(); (iteratorRelay != m_relaysList.end() && (command->getKeyword() != (*iteratorRelay)->getKeyword())); ++iteratorRelay)
            ++counter;

         parameters.set("rl" + boost::lexical_cast<std::string>(counter+1), stringState);

         if (iteratorRelay == m_relaysList.end())
            throw shared::exception::CException("Failed to identify the relay");

         shared::CDataContainer results = urlManager::setRelayState(m_configuration.getIPAddressWithSocket(),
                                                                     credentials,
                                                                     parameters);

         // TODO : Check the return of this function

         // historize the new value after sending and check that the value is well registered and return from the WES Equipment
         (*iteratorRelay)->set(newValue);
         keywordsToHistorize.push_back((*iteratorRelay));
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

   void CWESEquipment::setDeviceState(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize, 
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
   {}
}// namespace equipments