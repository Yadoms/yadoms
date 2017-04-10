#include "stdafx.h"
#include "WES.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>

#include "equipments/manuallyDeviceCreationException.hpp"
#include "equipments/noInformationException.hpp"
#include <shared/Log.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWES)


CWES::CWES():
   m_factory(boost::make_shared<CWESFactory>()),
   m_configuration(boost::make_shared<CWESConfiguration>())
{}

CWES::~CWES()
{}

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
   try {
      m_configuration->initializeWith(api->getConfiguration());

      setPluginState(api, EWESPluginState::kInitialization);
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initialization");
      m_ioManager = m_factory->loadConfiguration(api, m_configuration);

      if (m_ioManager->getMasterEquipment() == 0)
         setPluginState(api, EWESPluginState::kReady);
      else
      {
         m_ioManager->readAllDevices(api, true); // first reading
         setPluginState(api, EWESPluginState::kRunning);
         // Create the timer for refresh IOs
         m_refreshTimer = api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));
      }
   }
   catch (std::exception &e)
   {
      setPluginState(api, EWESPluginState::kInitializationError);
   }

   // the main loop
   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
      {
         YADOMS_LOG(information) << "Stop requested";
         setPluginState(api, EWESPluginState::kStop);
         return;
      }
      case kConnectionRetryTimer:
         break;
      case yApi::IYPluginApi::kEventUpdateConfiguration:
      {
         setPluginState(api, EWESPluginState::kupdateConfiguration);
         onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
         break;
      }
      case kRefreshStatesReceived:
      {
         YADOMS_LOG(information) << "Timer received";

         auto forceRefresh = false;

         try { forceRefresh = api->getEventHandler().getEventData<bool>(); }
         catch (shared::exception::CBadConversion&) {}

         // TODO : device list to parse and update
         m_ioManager->readAllDevices(api, false);
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         // Yadoms asks for device creation
         setPluginState(api, EWESPluginState::kupdateConfiguration);
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
         YADOMS_LOG(information) << "Manually device creation request received for device : " << request->getData().getDeviceName();
         try
         {
            // Creation of the device
            request->sendSuccess(m_factory->createDeviceManually(api, m_ioManager, request->getData()));
            setPluginState(api, EWESPluginState::kRunning);
         }
         catch (CManuallyDeviceCreationException& e)
         {
            request->sendError(e.what());
            setPluginState(api, EWESPluginState::kInitializationError);
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
            setPluginState(api, EWESPluginState::kInitializationError);
         }
         break;
      }
      default:
      {
         YADOMS_LOG(information) << "Unknown message id for pluginStateFaulty";
         break;
      }
      }
   }
}

void CWES::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

                                                // Update configuration
   m_configuration->initializeWith(newConfigurationData);
   //m_ioManager->OnConfigurationUpdate(api, m_configuration);
}

void CWES::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, EWESPluginState newState)
{
   if (m_pluginState != newState)
   {
      switch (newState)
      {
      case EWESPluginState::kInitialization:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "initialization");
         break;
      case EWESPluginState::kInitializationError:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
         break;
      case EWESPluginState::kReady:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "ready");
         break;
      case EWESPluginState::kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateconfiguration");
         break;
      case EWESPluginState::kAtLeastOneConnectionFaulty:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
         break;
      case EWESPluginState::kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case EWESPluginState::kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         YADOMS_LOG(error) << "this plugin status does not exist : " << newState;
         break;
      }

      m_pluginState = newState;
   }
}