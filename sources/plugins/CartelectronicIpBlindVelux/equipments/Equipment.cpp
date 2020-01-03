#include "stdafx.h"
#include "Equipment.h"
#include <shared/DataContainer.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>
#include "../urlManager.h"
#include "shared/http/HttpException.hpp"
#include "manuallyDeviceCreationException.hpp"

namespace equipments
{
	const int CEquipment::shuttersQty = 2;

	CEquipment::CEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                           const std::string& device,
		                   const shared::CDataContainer& deviceConfiguration)
      : m_deviceName(device),
        m_deviceType("CartelectronicIpBlindVelux"),
        m_deviceStatus(boost::make_shared<specificHistorizers::CdeviceStatus>("Status"))
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToDeclare;
      shared::CDataContainer details;

      try{
         keywordsToDeclare.push_back(m_deviceStatus);
         m_configuration.initializeWith(deviceConfiguration);
         deviceConfiguration.printToLog(YADOMS_LOG(information));

		 // Relay Configuration
		 for (auto counter = 0; counter < shuttersQty; ++counter) {
			 const auto temp = boost::make_shared<specificHistorizers::CVeluxCurtain>("shutter" + boost::lexical_cast<std::string>(counter));
			 m_shutters.push_back(temp);
			 keywordsToDeclare.push_back(temp);
		 }

         // Save names into details
         details.set("provider", "VRTIP");
         details.set("type", m_deviceType);

         //Déclaration of all IOs
         if (api->deviceExists(m_deviceName)){
            auto existingModel = api->getDeviceModel(m_deviceName);
            if (existingModel.empty())
               api->updateDeviceModel(m_deviceName, "VRTIP");

            api->updateDeviceDetails(m_deviceName, details);
            api->declareKeywords(m_deviceName, keywordsToDeclare);
         }
         else
            api->declareDevice(m_deviceName, "VRTIP", "VRTIP", keywordsToDeclare, details);
      }
      catch (shared::CHttpException& e){
         YADOMS_LOG(error) << e.what();
		 if (boost::contains(e.what(), "Timeout")) {
			 m_deviceStatus->set(specificHistorizers::EdeviceStatus::kTimeOut);
			 api->historizeData(m_deviceName, m_deviceStatus);
		 }
         throw e;
      }
      catch (std::exception& e){
         YADOMS_LOG(error) << e.what();
         throw CManuallyDeviceCreationException(e.what());
      }
   }

   std::string CEquipment::getDeviceName() const{
      return m_deviceName;
   }

   std::string CEquipment::getDeviceType() const{
      return m_deviceType;
   }

   void CEquipment::updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, bool forceHistorization)
   {
	   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize;

	   try {
		   std::string results = urlManager::getRelayState(
			   m_configuration.getIPAddressWithSocket());

		   YADOMS_LOG(information) << results;

		   // Reading relays - historize only on change value or when the historization is forced (initialization, for example)      
		   /*try {
			   updateSwitchValue(keywordsToHistorize, m_relaysList[0], results.get<bool>("RL1"), forceHistorization);
			   updateSwitchValue(keywordsToHistorize, m_relaysList[1], results.get<bool>("RL2"), forceHistorization);
		   }
		   catch (std::exception& e) {
			   YADOMS_LOG(warning) << "Exception reading relays" << e.what();
		   }*/

		   setDeviceState(keywordsToHistorize, specificHistorizers::EdeviceStatus::kOk);
		   api->historizeData(m_deviceName, keywordsToHistorize);
	   }
	   catch (shared::CHttpException& e) {
		   if (boost::contains(e.what(), "Timeout")) {
			   setDeviceState(keywordsToHistorize, specificHistorizers::EdeviceStatus::kTimeOut);
			   api->historizeData(m_deviceName, keywordsToHistorize);
		   }
	   }
	   catch (std::exception& e) {
		   YADOMS_LOG(error) << e.what();
		   setDeviceState(keywordsToHistorize, specificHistorizers::EdeviceStatus::kError);
		   api->historizeData(m_deviceName, keywordsToHistorize);
	   }
   }

   void CEquipment::updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const shared::CDataContainer& newConfiguration,
                                        const int refreshEvent)
   {
      m_configuration.initializeWith(newConfiguration);
      api->updateDeviceConfiguration(m_deviceName, newConfiguration);
      YADOMS_LOG(information) << "Configuration updated for " << m_deviceName;
   }

   void CEquipment::sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& keyword,
                                boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize;

      try{
         shared::CDataContainer parameters;
         std::string stringState;
         auto counter = 0;

		 YADOMS_LOG(information) << command->getBody();

		 specificHistorizers::EVeluxCurtainCommand newValue(command->getBody());
         std::vector<boost::shared_ptr<specificHistorizers::CVeluxCurtain>>::const_iterator iteratorRelay;
         for (iteratorRelay = m_shutters.begin(); (iteratorRelay != m_shutters.end() && (command->getKeyword() != (*iteratorRelay)->getKeyword())); ++iteratorRelay)
            ++counter;

         parameters.set("shutter" + boost::lexical_cast<std::string>(counter + 1), boost::lexical_cast<std::string>(newValue.toInteger()));

		 // Adding the activation delay only if the command is different of Stop
		 if (newValue != specificHistorizers::EVeluxCurtainCommand::kStop) {
			 parameters.set("tm" + boost::lexical_cast<std::string>(counter + 1), m_configuration.getShutterDelay(counter + 1));
		 }

         if (iteratorRelay == m_shutters.end())
            throw shared::exception::CException("Failed to identify the shutter");

         auto results = urlManager::setRelayState(
			 m_configuration.getIPAddressWithSocket(),
			 parameters);

		 YADOMS_LOG(information) << results;

         setDeviceState(keywordsToHistorize, specificHistorizers::EdeviceStatus::kOk);
         api->historizeData(m_deviceName, keywordsToHistorize);
      }
      catch (std::exception& e){
         keywordsToHistorize.clear();
         setDeviceState(keywordsToHistorize, specificHistorizers::EdeviceStatus::kError);
         api->historizeData(m_deviceName, keywordsToHistorize);
         YADOMS_LOG(error) << e.what();
      }
   }

   void CEquipment::setDeviceState(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywordsToHistorize,
                                      specificHistorizers::EdeviceStatus newState) const
   {
      if (m_deviceStatus->get() != newState){
         m_deviceStatus->set(newState);
         keywordsToHistorize.push_back(m_deviceStatus);
         YADOMS_LOG(trace) << "device state " << m_deviceName << " set to " << newState.toString();
      }
   }

   specificHistorizers::EdeviceStatus CEquipment::getStatus() const{
      return m_deviceStatus->get();
   }

   CEquipment::~CEquipment()
   {}
}// namespace equipments