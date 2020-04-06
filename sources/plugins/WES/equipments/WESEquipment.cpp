#include "stdafx.h"
#include "WESEquipment.h"
#include "Helpers.h"
#include <shared/DataContainer.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "../urlManager.h"
#include "tooLowRevisionException.hpp"
#include "manuallyDeviceCreationException.hpp"

namespace equipments
{
   const CWESEquipment::WESIOMapping CWESEquipment::WESv1 = {2, 3, 2, 2, 2, 0};
   const CWESEquipment::WESIOMapping CWESEquipment::WESv2 = {2, 3, 2, 4, 4, 4};

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
      std::string relayName[2], TICName[3], ClampName[4], AnalogName[4], inputName[2];
      equipments::ContractAvailable contract[3];
      subdevices::EUnit PulseType[4];
      std::string PulseName[4];
      std::string counterId[3];
      shared::CDataContainer details;

      try
      {
         keywordsToDeclare.push_back(m_deviceStatus);
         m_configuration.initializeWith(deviceConfiguration);
         deviceConfiguration.printToLog(YADOMS_LOG(information));

         shared::CDataContainer credentials;

		 if (m_configuration.credentialActivated()) {
			 credentials.set("user", m_configuration.getUser());
			 credentials.set("password", m_configuration.getPassword());
		 }

         // request to obtain the WES revision
         std::string CGXfileName = "WESVERSION.CGX";
		 auto results = urlManager::readFileState(
			 m_configuration.getIPAddressWithSocket(),
			 credentials,
			 CGXfileName,
			 m_httpContext,
             urlManager::httpRequestCreationTimeout);

         results.printToLog(YADOMS_LOG(trace));

         // get the revision, for E/S numbers
         m_version = results.get<int>("version");

         if (m_version == 1)
            m_WESIOMapping = WESv1;
         else if (m_version == 2)
            m_WESIOMapping = WESv2;
         else
            throw shared::exception::CException("WES Revision not properly set : " + boost::lexical_cast<std::string>(m_version));

         // get the firmware revision to desactivate the plugin if the revision is too low
		 checkRevision(results.get<std::string>("version_serveur"));

         if (pluginConfiguration->isRetrieveNamesFromdevice())
         {
            // request to obtain names
            CGXfileName = "WESNAMES" + boost::lexical_cast<std::string>(m_version) + ".CGX";
            results = urlManager::readFileState(
				m_configuration.getIPAddressWithSocket(),
                credentials,
                CGXfileName,
				m_httpContext,
                urlManager::httpRequestCreationTimeout);

            results.printToLog(YADOMS_LOG(information));

			for (auto index = 0; index < m_WESIOMapping.ticQty; ++index) {
				contract[index] = results.get<equipments::ContractAvailable>("CPT" + boost::lexical_cast<std::string>(index + 1) + "_abo_name");
				counterId[index] = results.get<std::string>("CPT" + boost::lexical_cast<std::string>(index + 1) + "_adco");
				TICName[index] = results.get<std::string>("CPT" + boost::lexical_cast<std::string>(index + 1) + "_name");
			}

            relayName[0] = results.get<std::string>("RL1");
            relayName[1] = results.get<std::string>("RL2");

            inputName[0] = results.get<std::string>("I1");
            inputName[1] = results.get<std::string>("I2");

            ClampName[0] = results.get<std::string>("nPCE1");
            ClampName[1] = results.get<std::string>("nPCE2");

            if (m_version == 2) {
               ClampName[2] = results.get<std::string>("nPCE3");
               ClampName[3] = results.get<std::string>("nPCE4");
            }

            AnalogName[0] = results.get<std::string>("ANA1");
            AnalogName[1] = results.get<std::string>("ANA2");
            AnalogName[2] = results.get<std::string>("ANA3");
            AnalogName[3] = results.get<std::string>("ANA4");

            PulseName[0] = results.get<std::string>("npls1");
            PulseName[1] = results.get<std::string>("npls2");
            PulseType[0] = results.get<subdevices::EUnit>("PLSU1");
            PulseType[1] = results.get<subdevices::EUnit>("PLSU2");

            if (m_version == 2) {
               PulseName[2] = results.get<std::string>("npls3");
               PulseName[3] = results.get<std::string>("npls4");
               PulseType[2] = results.get<subdevices::EUnit>("PLSU3");
               PulseType[3] = results.get<subdevices::EUnit>("PLSU4");
            }
         }
         else // Defaults names
         {
            contract[0] = equipments::NotAvailable;
            contract[1] = equipments::NotAvailable;
            counterId[0] = "000000000000";
            counterId[1] = "000000000000";
            counterId[2] = "000000000000";

            relayName[0] = "R01";
            relayName[1] = "R02";

            inputName[0] = "I1";
            inputName[1] = "I2";

            TICName[0] = "TIC01";
            TICName[1] = "TIC02";
            TICName[2] = "TIC03";

            ClampName[0] = "PCE1";
            ClampName[1] = "PCE2";

            if (m_version == 2) {
               ClampName[2] = "PCE3";
               ClampName[3] = "PCE4";
            }

            AnalogName[0] = "ANA1";
            AnalogName[1] = "ANA2";
            AnalogName[2] = "ANA3";
            AnalogName[3] = "ANA4";

            PulseName[0] = "pls1";
            PulseName[1] = "pls2";
            PulseType[0] = subdevices::EUnit::undefined;
            PulseType[1] = subdevices::EUnit::undefined;

            if (m_version == 2) {
               PulseName[2] = "pls3";
               PulseName[3] = "pls4";
               PulseType[2] = subdevices::EUnit::undefined;
               PulseType[3] = subdevices::EUnit::undefined;
            }
         }

         // Relay Configuration
         for (auto counter = 0; counter < m_WESIOMapping.relayQty; ++counter){
            const auto temp = boost::make_shared<yApi::historization::CSwitch>(relayName[counter],
                                                                         yApi::EKeywordAccessMode::kGetSet);
            m_relaysList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // Input Configuration
         for (auto counter = 0; counter < m_WESIOMapping.inputQty; ++counter){
            const auto temp = boost::make_shared<yApi::historization::CSwitch>(inputName[counter],
                                                                         yApi::EKeywordAccessMode::kGet);
            m_inputList.push_back(temp);
            keywordsToDeclare.push_back(temp);
         }

         // TIC Counters Configuration
         for (auto index = 0; index < m_WESIOMapping.ticQty; ++index){
			const auto temp = boost::make_shared<equipments::CTIC>(api,
																m_deviceName + " - " + TICName[index],
																counterId[index],
																contract[index]);

			details.set("TIC" + boost::lexical_cast<std::string>(index), m_deviceName + " - " + TICName[index]);
			m_TICList.push_back(temp);
         }

         // Pulse Counters Configuration
         for (auto counter = 0; counter < m_WESIOMapping.pulseQty; ++counter){
            const auto temp = boost::make_shared<subdevices::CPulse>(api,
                                                               keywordsToDeclare,
                                                               m_deviceName,
                                                               PulseName[counter],
                                                               PulseType[counter]);
            m_PulseList.push_back(temp);
         }

         // Current clamp Configuration
         for (auto counter = 0; counter < m_WESIOMapping.clampQty; ++counter){
            const auto temp = boost::make_shared<subdevices::CClamp>(api,
                                                               keywordsToDeclare,
                                                               m_deviceName,
                                                               ClampName[counter]);
            m_ClampList.push_back(temp);
         }

         // Analog Values Configuration
		 if (m_configuration.isAnalogInputsActivated() && (m_version == 2)) {
			 for (auto counter = 0; counter < m_WESIOMapping.anaQty; ++counter){
				 const auto temp = boost::make_shared<subdevices::CAnalog>(api,
					 keywordsToDeclare,
					 m_configuration.analogInputsType(counter + 1),
					 m_deviceName,
					 AnalogName[counter]);
				 m_AnalogList.push_back(temp);
			 }
		 }

         // Save names into details
         details.set("provider", "WES");
         details.set("type", m_deviceType);

         //D�claration of all IOs
         if (api->deviceExists(m_deviceName)){
            auto existingModel = api->getDeviceModel(m_deviceName);
            if (existingModel.empty())
               api->updateDeviceModel(m_deviceName, "WES");

            api->updateDeviceDetails(m_deviceName, details);
            api->declareKeywords(m_deviceName, keywordsToDeclare);
         }
         else
            api->declareDevice(m_deviceName, "WES", "WES", keywordsToDeclare, details);
      }
	  catch (CtooLowRevisionException& e) {
		  YADOMS_LOG(error) << e.what();
		  throw e;
	  }
      catch (shared::exception::CException& e){
		  if (api->deviceExists(m_deviceName)) {
			  if (boost::contains(e.what(), "Timeout")) {
				  YADOMS_LOG(error) << e.what();
				  YADOMS_LOG(error) << "Passage 1";

				  m_deviceStatus->set(specificHistorizers::EWESdeviceStatus::kTimeOut);
				  api->historizeData(m_deviceName, m_deviceStatus);

				  details = api->getDeviceDetails(m_deviceName);  // We read TIC device names, to set the state for each
				  m_WESIOMapping = WESv2;                         // default mapping

				  // TIC Counters SetDevice Timeout if exist ! At creation time out, TIC devices doesn't exists
				  for (auto counter = 0; counter < m_WESIOMapping.ticQty; ++counter) {
					  if (details.exists("TIC" + boost::lexical_cast<std::string>(counter))) {
						  TICName[counter] = details.get<std::string>("TIC" + boost::lexical_cast<std::string>(counter));
						  boost::make_shared<equipments::CTIC>(api, TICName[counter]);
					  }
				  }
				  throw std::runtime_error("Time out");
			  }
		  }

		  throw e;
      }
      catch (std::exception& e){
         YADOMS_LOG(error) << e.what();
         throw CManuallyDeviceCreationException(e.what());
      }
   }

   std::string CWESEquipment::getDeviceName() const{
      return m_deviceName;
   }

   std::string CWESEquipment::getDeviceType() const{
      return m_deviceType;
   }

   void CWESEquipment::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                                        bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize;

      try{
         if (m_version == 0 || m_version > 2)
            throw shared::exception::CException("m_version is not set properly : " + boost::lexical_cast<std::string>(m_version));

         const auto CGXfileName = "WESVALUES" + boost::lexical_cast<std::string>(m_version) + ".CGX";

         shared::CDataContainer credentials;

		 if (m_configuration.credentialActivated()) {
			 credentials.set("user", m_configuration.getUser());
			 credentials.set("password", m_configuration.getPassword());
		 }

         auto results = urlManager::readFileState(
			 m_configuration.getIPAddressWithSocket(),
             credentials,
             CGXfileName,
			 m_httpContext,
             urlManager::httpRequestWESTimeout);

         results.printToLog(YADOMS_LOG(trace));

         // Reading relays - historize only on change value or when the historization is forced (initialization, for example)      
         try{
            updateSwitchValue(keywordsToHistorize, m_relaysList[0], results.get<bool>("RL1"), forceHistorization);
            updateSwitchValue(keywordsToHistorize, m_relaysList[1], results.get<bool>("RL2"), forceHistorization);
         }
         catch (std::exception& e){
            YADOMS_LOG(warning) << "Exception reading relays" << e.what();
         }

         try{
            updateSwitchValue(keywordsToHistorize, m_inputList[0], results.get<bool>("I1"), forceHistorization);
            updateSwitchValue(keywordsToHistorize, m_inputList[1], results.get<bool>("I2"), forceHistorization);
         }
         catch (std::exception& e){
            YADOMS_LOG(warning) << "Exception reading inputs" << e.what();
         }

         //Reading clamp values
         for (auto counter = 0; counter < m_WESIOMapping.clampQty; ++counter){
            try{
               m_ClampList[counter]->updateFromDevice(api,
                                                      keywordsToHistorize,
                                                      results.get<Poco::Int64>("WHPC" + boost::lexical_cast<std::string>(counter + 1) + "_val"));
            }
            catch (std::exception& e){
               YADOMS_LOG(warning) << "Error reading clamp " << "WHPC" + boost::lexical_cast<std::string>(counter + 1) + "_val" << e.what();
            }
         }

         //Reading pulse values
         for (auto counter = 0; counter < m_WESIOMapping.pulseQty; ++counter){
            try{
               m_PulseList[counter]->updateFromDevice(api,
                                                      keywordsToHistorize,
                                                      results.get<subdevices::EUnit>("PLSU" + boost::lexical_cast<std::string>(counter + 1)),
                                                      results.get<double>("actuel" + boost::lexical_cast<std::string>(counter + 1) + "_val"));
            }
            catch (std::exception& e){
               YADOMS_LOG(warning) << "Error reading pulse " << "actuel" + boost::lexical_cast<std::string>(counter + 1) + "_val :" << e.what();
            }
         }

         //Reading analog values
         if (m_configuration.isAnalogInputsActivated() && (m_version == 2)){
			for (auto counter = 0; counter < m_WESIOMapping.anaQty; ++counter) {
				try {
					auto value = results.get<double>("ad"+boost::lexical_cast<std::string>(counter + 1));
					m_AnalogList[counter]->updateFromDevice(api, keywordsToHistorize, value);
				}
				catch (std::exception& e) {
					YADOMS_LOG(warning) << "Error reading ad " + boost::lexical_cast<std::string>(counter + 1) + " value :" << e.what();
				}
			}
         }

         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kOk);
         api->historizeData(m_deviceName, keywordsToHistorize);

         // TIC Counters Values -> independant from the others keywords
         for (auto counter = 0; counter < m_WESIOMapping.ticQty; ++counter){
			 std::vector<Poco::Int64> counters;

			for (auto index = 0; index < CTIC::TICCountersNb; ++index) {
				counters.push_back(results.get<Poco::Int64>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_INDEX_" + boost::lexical_cast<std::string>(index + 1)));
			}

			m_TICList[counter]->updateFromDevice(api,
				m_deviceStatus->get(),
				results.get<equipments::ContractAvailable>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_abo_name"),
				results.get<std::string>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_adco"),
				results.get<int>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_PTarif"),
				results.get<unsigned int>("CPT" + boost::lexical_cast<std::string>(counter + 1) + "_P"),
				counters,
				results.get<int>("DEMAIN_" + boost::lexical_cast<std::string>(counter + 1)));
         }
      }
      catch (std::exception& e){
		  if (boost::contains(e.what(), "Timeout")) {
			  setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kTimeOut);
			  api->historizeData(m_deviceName, keywordsToHistorize);

			  for (auto counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
				  m_TICList[counter]->setDeviceState(api, specificHistorizers::EWESdeviceStatus::kTimeOut);
		  }
		  else {
			  YADOMS_LOG(error) << e.what();
			  setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kError);
			  api->historizeData(m_deviceName, keywordsToHistorize);
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
                                           const shared::CDataContainer& newConfiguration,
                                           const int refreshEvent)
   {
      m_configuration.initializeWith(newConfiguration);
      api->updateDeviceConfiguration(m_deviceName, newConfiguration);
      YADOMS_LOG(information) << "Configuration updated for " << m_deviceName;

      // For Analog
      //Reading analog values
      if (m_configuration.isAnalogInputsActivated()){
         // Analog Values Configuration
		  std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToDeclare;
         for (auto counter = 0; counter < m_WESIOMapping.anaQty; ++counter){
            auto analogConfigurationType = m_configuration.analogInputsType(counter + 1);
            // remove the keyword, if the type is different. It should be recreated automatically at next reading
            if (m_AnalogList[counter]->type() != analogConfigurationType){
               api->removeKeyword(m_deviceName, m_AnalogList[counter]->name());
			   const auto temp = boost::make_shared<subdevices::CAnalog>(api,
				   keywordsToDeclare,
				   analogConfigurationType,
				   m_deviceName,
				   m_AnalogList[counter]->name());
			   m_AnalogList[counter] = temp;
            }
         }

		 // Declare new keywords
		 if (keywordsToDeclare.size() != 0) {
			 api->declareKeywords(m_deviceName, keywordsToDeclare);
			 api->getEventHandler().postEvent(refreshEvent);
		 }
      }
      else {
         for (auto counter = 0; counter < m_WESIOMapping.anaQty; ++counter){
            // If keyword already exists, we delete it
			 if (m_AnalogList[counter] || api->keywordExists(m_deviceName, m_AnalogList[counter]->name())) {
				 api->removeKeyword(m_deviceName, m_AnalogList[counter]->name());
				 m_AnalogList.erase(m_AnalogList.begin() + counter + 1);
			 }
         }
      }
   }

   void CWESEquipment::sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const std::string& keyword,
                                   boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize;

      try{
         shared::CDataContainer credentials;
         shared::CDataContainer parameters;
         std::string stringState;
         auto counter = 0;

		 if (m_configuration.credentialActivated()) {
			 credentials.set("user", m_configuration.getUser());
			 credentials.set("password", m_configuration.getPassword());
		 }

         const auto newValue = boost::lexical_cast<bool>(command->getBody());
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

         auto results = urlManager::setRelayState(
			 m_configuration.getIPAddressWithSocket(),
             credentials,
             parameters,
			 m_httpContext);

         // For security, we check if the results contain the value, with the corresponding new state
         if (results.containsChild("Relai1")){
            if (results.get<std::string>("Relai1") == "ON" && !m_relaysList[0]->get()){
               m_relaysList[0]->set(true);
               keywordsToHistorize.push_back(m_relaysList[0]);
            }
            else if (results.get<std::string>("Relai1") == "OFF" && m_relaysList[0]->get()){
               m_relaysList[0]->set(false);
               keywordsToHistorize.push_back(m_relaysList[0]);
            }
         }

         if (results.containsChild("Relai2")){
            if (results.get<std::string>("Relai2") == "ON" && !m_relaysList[1]->get()){
               m_relaysList[1]->set(true);
               keywordsToHistorize.push_back(m_relaysList[1]);
            }
            else if (results.get<std::string>("Relai2") == "OFF" && m_relaysList[1]->get()){
               m_relaysList[1]->set(false);
               keywordsToHistorize.push_back(m_relaysList[1]);
            }
         }

         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kOk);
         api->historizeData(m_deviceName, keywordsToHistorize);
      }
      catch (std::exception& e){
         keywordsToHistorize.clear();
         setDeviceState(keywordsToHistorize, specificHistorizers::EWESdeviceStatus::kError);
         api->historizeData(m_deviceName, keywordsToHistorize);
         YADOMS_LOG(error) << e.what();
      }
   }

   void CWESEquipment::setDeviceState(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywordsToHistorize,
                                      specificHistorizers::EWESdeviceStatus newState) const
   {
      if (m_deviceStatus->get() != newState){
         m_deviceStatus->set(newState);
		 if (std::find(keywordsToHistorize.begin(), keywordsToHistorize.end(), m_deviceStatus) == keywordsToHistorize.end()) {
			 keywordsToHistorize.push_back(m_deviceStatus);
		 }
         YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
      }
   }

   specificHistorizers::EWESdeviceStatus CWESEquipment::getStatus() const{
      return m_deviceStatus->get();
   }

   void CWESEquipment::remove(boost::shared_ptr<yApi::IYPluginApi> api){
      // Delete 2 others devices
      for (auto counter = 0; counter < m_WESIOMapping.ticQty; ++counter)
         m_TICList[counter]->remove(api);
   }

   CWESEquipment::~CWESEquipment()
   {}
}// namespace equipments