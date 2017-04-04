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

   auto factory = boost::make_shared<CWESFactory>();

   // Lauch the state
   m_pluginState.process_event(EvtStart(api, factory));

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
      {
         m_pluginState.process_event(EvtStop(api));
         m_pluginState.stop();
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

            // TODO : device list to parse and update
         }
         catch (std::exception &e) // final catch for other reason
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         // Yadoms asks for device creation
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
         YADOMS_LOG(information) << "Manually device creation request received for device : " << request->getData().getDeviceName();
         try
         {
            // Creation of the device
            request->sendSuccess(factory->createDeviceManually(api, request->getData()));

            m_pluginState.process_event(EvtConfigurationUpdated(api));
         }
         catch (CManuallyDeviceCreationException& e)
         {
            request->sendError(e.what());
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      case yApi::IYPluginApi::kEventDeviceRemoved:
      {
         try {
            auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
            factory->removeDevice(api, device->device());
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      case yApi::IYPluginApi::kBindingQuery:
      {
         // Yadoms ask for a binding query 
         try {
            //TODO : To be used with WES extensions
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());

         try {
            // TODO : To be execute
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Exception : " << e.what();
         }
         break;
      }
      default:
      {
         YADOMS_LOG(error) << "Unknown message id";
         break;
      }
      }
   }
}