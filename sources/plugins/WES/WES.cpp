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

#include "stateCommonDeclaration.hpp"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWES)


CWES::CWES()
   : m_deviceName("WES")
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
   // Start the MSM
   m_pluginState.start();

   // Lauch the state
   m_pluginState.process_event(EvtStart(api));
   
   //auto factory = m_pluginState.get_attribute(m_factory);
   //m_ioManager = factory->getIOManager();

   // for debug purpose only
   // YADOMS_LOG(information) << "m_pluginState.current_state() : " << *m_pluginState.current_state();

   //auto essai = m_configuration.getIPAddressWithSocket();

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
      {
         m_pluginState.process_event(EvtStop);
         m_pluginState.stop();
         //api->setPluginState(yApi::historization::EPluginState::kStopped);
         return;
      }
      case kConnectionRetryTimer:
         m_pluginState.process_event(EvtDisconnected(api));
         break;
      case kAnswerTimeout:
      {
         m_pluginState.process_event(EvtConnectionLost(api));
         break;
      }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
      {
         m_pluginState.process_event(EvtNewConfigurationRequested(api, api->getEventHandler().getEventData<shared::CDataContainer>()));
         break;
      }
      case kRefreshStatesReceived:
      {
         YADOMS_LOG(information) << "Timer received";

         try {
            auto forceRefresh = false;

            try { forceRefresh = api->getEventHandler().getEventData<bool>(); }
            catch (shared::exception::CBadConversion&) {}

            m_ioManager->readAllIOFromDevice(api, forceRefresh);
         }
         catch (std::exception &e) // final catch for other reason
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      default:
         break;
      }
   }

   /*
   try {
      m_configuration.initializeWith(api->getConfiguration());

      //Factory : Creation of all the needed
      m_factory = boost::make_shared<CWESFactory>(api, m_deviceName, m_configuration);

      m_ioManager = m_factory->getIOManager();

	  // First connection
	  api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(00));
      
      // Timer used to read periodically IOs from the WES
	  m_refreshTimer = api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));
	  m_refreshTimer->stop();

      api->setPluginState(yApi::historization::EPluginState::kRunning);
      YADOMS_LOG(information) << "WES plugin is running..." ;
   }
   catch (...)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
      YADOMS_LOG(error) << "WES plugin initialization error..." ;
   }

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {

      case kRefreshStatesReceived:
      {
         YADOMS_LOG(information) << "Timer received" ;

         try {
            auto forceRefresh = false;

               try { forceRefresh = api->getEventHandler().getEventData<bool>(); }
            catch (shared::exception::CBadConversion&) { }

            m_ioManager->readAllIOFromDevice(api, forceRefresh);
         }
         catch (std::exception &e) // final catch for other reason
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
         }
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         // Yadoms asks for device creation
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
         YADOMS_LOG(information) << "Manually device creation request received for device :" << request->getData().getDeviceName() ;
         try
         {
            // Creation of the device
            request->sendSuccess(m_factory->createDeviceManually(api, request->getData()));

            // Send an event to refresh all IOs
            api->getEventHandler().postEvent<bool>(kRefreshStatesReceived, true);
         }
         catch (CManuallyDeviceCreationException& e)
         {
            request->sendError(e.what());
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
         }
         break;
      }
      case yApi::IYPluginApi::kEventDeviceRemoved:
      {
         try {
            auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
            m_factory->removeDevice(api, device->device());
            m_ioManager->removeDevice(api, device->device());
            YADOMS_LOG(information) << device->device() << " was removed" ;
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
         }
         break;
      }
      case yApi::IYPluginApi::kBindingQuery:
      {
         // Yadoms ask for a binding query 
        try {
            auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest> >();

            if (data->getData().getQuery() == "X8R")
                data->sendSuccess(m_factory->bindSlotsX8R());
            else if (data->getData().getQuery() == "X8D")
                data->sendSuccess(m_factory->bindSlotsX8D());
            else if (data->getData().getQuery() == "X24D")
                data->sendSuccess(m_factory->bindSlotsX24D());
            else
            {
                std::string errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
                data->sendError(errorMessage);
                YADOMS_LOG(error) << errorMessage ;
            }
        }
        catch (std::exception &e)
        {
            YADOMS_LOG(information) << "Unknow error : " << e.what() ;
        }
        break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());

         try {
            m_ioManager->onCommand(api, command);
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Exception : " << e.what() ;
         }
         break;
      }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id" ;
            break;
         }
      }
   }*/
}