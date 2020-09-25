#include "stdafx.h"
#include "WES.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>

#include "equipments/manuallyDeviceCreationException.hpp"
#include "equipments/tooLowRevisionException.hpp"
#include <shared/Log.h>
#include <boost/property_tree/xml_parser.hpp>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWES)


CWES::CWES()
   : m_configuration(boost::make_shared<CWESConfiguration>()),
     m_factory(boost::make_shared<CWESFactory>()),
     m_pluginState(kUndefined)
{
}

CWES::~CWES()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kRefreshStatesReceived,
   kConnectionRetryTimer,
   kAnswerTimeout
};

void CWES::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      setPluginState(api, kInitialization);
      m_configuration->initializeWith(api->getConfiguration());
      m_ioManager = m_factory->loadConfiguration(api, m_configuration);

      // Create timer for refresh IOs
      m_refreshTimer = api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));

      if (m_ioManager->getMasterEquipment() == 0 && m_ioManager->getWaitingEquipment() == 0){
         setPluginState(api, kReady);
         m_refreshTimer->stop();
      }
      else{
         m_refreshTimer->start();
		 api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0)); // Send an immediat refresh value event

         if (m_ioManager->getWaitingEquipment() != 0)
            setPluginState(api, kAtLeastOneConnectionFaulty);
         else
            setPluginState(api, kRunning);
      }
   }
   catch (std::exception&)
   {
      setPluginState(api, kAtLeastOneConnectionFaulty);
   }

   // the main loop
   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested";
            setPluginState(api, kStop);
            return;
         }
      case kConnectionRetryTimer:
         break;
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            setPluginState(api, kupdateConfiguration);
            onUpdateConfiguration(api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>());
            setPluginState(api, kRunning);
            break;
         }
      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // Command received from Yadoms
            auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());

            try
            {
               m_ioManager->onCommand(api, command);
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(error) << "Fail to send command " << yApi::IDeviceCommand::toString(command) << ", error : " << e.what();
            }
            break;
         }
      case kRefreshStatesReceived:
         {
            YADOMS_LOG(information) << "Timer received";
            auto forceRefresh = false;

            try{
               forceRefresh = api->getEventHandler().getEventData<bool>();
            }
            catch (std::exception&)
            {}

            if (m_ioManager->getWaitingEquipment() != 0)
               m_ioManager->tryMissingEquipment(api, m_configuration);

            m_ioManager->readAllDevices(api, m_configuration, forceRefresh);

            analyzePluginState(api);
            break;
         }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
         {
            // Yadoms asks for device creation
            setPluginState(api, kupdateConfiguration);
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
            YADOMS_LOG(information) << "Manually device creation request received for device : " << request->getData().getDeviceName();
            try{
               if (!m_ioManager->deviceAlreadyExist(request->getData().getDeviceName())){
                  // Creation of the device only when the name doesn't already exist
                  m_factory->createDeviceManually(api, m_ioManager, request->getData(), m_configuration);
                  request->sendSuccess();

                  // ask immediately for reading values
                  api->getEventHandler().postEvent<bool>(kRefreshStatesReceived, true);

                  if (m_ioManager->getMasterEquipment() != 0){
                     m_refreshTimer->start();
                     setPluginState(api, kRunning);
                  }
               }
               else{
                  request->sendError("device name already exist");
               }
            }
            catch (CtooLowRevisionException& e){
               request->sendError(e.what());
               setPluginState(api, kserverRevisionTooLow);
            }
            catch (std::exception& e){
               YADOMS_LOG(error) << "Unknow error : " << e.what();
               request->sendError(e.what());
               setPluginState(api, kmanuallyCreationDeviceFailed);
            }
            break;
         }
      case yApi::IYPluginApi::kEventDeviceRemoved:
         {
            try{
               auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved>>();

               if (m_ioManager){
                  m_ioManager->removeDevice(api, device->device());
                  YADOMS_LOG(information) << device->device() << " is removed";

                  // Check if no module present
                  if (m_ioManager->getMasterEquipment() == 0){
                     m_refreshTimer->stop(); // No more refresh timer
                     setPluginState(api, kReady); // State ready
                  }
               }
               else
               YADOMS_LOG(error) << "Cannot remove the device " << device->device() << ". IO Manager is not initialized";
            }
            catch (std::exception& e){
               YADOMS_LOG(information) << "Unknow error : " << e.what();
            }
            break;
         }
      case yApi::IYPluginApi::kBindingQuery:
         {
            // Yadoms ask for a binding query 
            try
            {
               auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest>>();

               if (data->getData().getQuery() == "wes")
               {
                  data->sendSuccess(m_ioManager->bindMasterDevice());
               }
               else
               {
                  std::string errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
                  data->sendError(errorMessage);
                  YADOMS_LOG(error) << errorMessage;
               }
            }
            catch (std::exception& e)
            {
               YADOMS_LOG(information) << "Unknow error : " << e.what();
            }
            break;
         }
      case yApi::IYPluginApi::kEventExtraQuery:
         {
            // TODO : To be developped when received directly information from the WES
            // Check the IP address to know how send us data

            // Extra-command was received from Yadoms
            auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery>>();

            if (extraQuery)
            {
               YADOMS_LOG(information) << "Extra command received : " << extraQuery->getData()->query();

               if (extraQuery->getData()->query() == "wes")
               {
                  YADOMS_LOG(information) << "Simple command received";
               }
            }
            // Extra-query can return success or error indicator. In case of success, can also return data.
            // Return here a success without data (=empty container)
            extraQuery->sendSuccess(shared::CDataContainer::make());
            break;
         }
      case yApi::IYPluginApi::kSetDeviceConfiguration:
         {
            // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
            auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();

            setPluginState(api, kupdateConfiguration);
            m_ioManager->OnDeviceConfigurationUpdate(api,
                                                     deviceConfiguration->name(),
                                                     deviceConfiguration->configuration(),
                                                     kRefreshStatesReceived);

            setPluginState(api, kRunning);

            break;
         }
      default:
         {
            YADOMS_LOG(information) << "Unknown message id received";
            break;
         }
      }
   }
}

void CWES::onUpdateConfiguration(const boost::shared_ptr<shared::CDataContainer>& newConfigurationData) const
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData->empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated
   m_configuration->initializeWith(newConfigurationData); // Update configuration
}

void CWES::analyzePluginState(boost::shared_ptr<yApi::IYPluginApi> api)
{
   auto newState = kRunning; // default state

   for (const auto& devicesStatus : m_ioManager->getMasterdeviceStates()){
      if (devicesStatus == specificHistorizers::EWESdeviceStatus::kTimeOut ||
         devicesStatus == specificHistorizers::EWESdeviceStatus::kError){
         newState = kAtLeastOneConnectionFaulty;
      }
   }

   if (m_ioManager->getWaitingEquipment() != 0) {
	   YADOMS_LOG(error) << "Waiting " << m_ioManager->getWaitingEquipment() << " equipment(s)";
	   newState = kAtLeastOneConnectionFaulty;
   }

   setPluginState(api, newState);
}

void CWES::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, EWESPluginState newState)
{
   if (m_pluginState != newState)
   {
      switch (newState)
      {
      case kInitialization:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "initialization");
         break;
      case kInitializationError:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
         break;
      case kReady:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "ready");
         break;
      case kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateconfiguration");
         break;
      case kAtLeastOneConnectionFaulty:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "kAtLeastOneConnectionFaulty");
         break;
      case kmanuallyCreationDeviceFailed:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "manuallyCreationError");
         break;
      case kserverRevisionTooLow:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "serverRevisionTooLow");
         break;
      case kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         YADOMS_LOG(error) << "this plugin status does not exist : " << newState;
         break;
      }

      m_pluginState = newState;
   }
}

