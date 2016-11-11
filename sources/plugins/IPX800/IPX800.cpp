#include "stdafx.h"
#include "IPX800.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CIPX800)


CIPX800::CIPX800()
   : m_deviceName("IPX800")
{}

CIPX800::~CIPX800()
{}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kRefreshStatesReceived
};

void CIPX800::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "IPX800 is starting..." << std::endl;
      
   try {
      m_configuration.initializeWith(api->getConfiguration());

      //Factory : Creation of all the needed
      m_factory = boost::make_shared<CIPX800Factory>(api, m_deviceName, m_configuration);

      m_ioManager = m_factory->getIOManager();

      // Timer used to read periodically IOs from the IPX800
      api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      api->setPluginState(yApi::historization::EPluginState::kRunning);
      std::cout << "IPX800 plugin is running..." << std::endl;
   }
   catch (...)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
      std::cerr << "IPX800 plugin initialization error..." << std::endl;
   }

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            std::cout << "Stop requested" << std::endl;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            api->setPluginState(yApi::historization::EPluginState::kRunning);
            break;
         }
      case kRefreshStatesReceived:
      {
         std::cout << "Timer received" << std::endl;
         //Value received from DI
         //if (!initializationError)
            m_ioManager->readAllIOFromDevice(api);
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         //TODO : A finir le catch, implémenter le CManuallyDeviceCreationException

         // Yadoms asks for device creation
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
         std::cout << "Manually device creation request received for device :" << request->getData().getDeviceName() << std::endl;
         try
         {
            // Creation of the device
            request->sendSuccess(m_factory->createDeviceManually(api, request->getData()));

            // read IOs juste after to fill all new IOs.
            m_ioManager->readAllIOFromDevice(api);
         }
         catch (/*CManuallyDeviceCreationException& e*/...)
         {
            //request->sendError(e.what());
         }
         
         break;
      }
      case yApi::IYPluginApi::kEventDeviceRemoved:
      {
         auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
         std::cout << device->device() << " was removed" << std::endl;
         m_ioManager->removeDevice(api, device->device());
         break;
      }
      case yApi::IYPluginApi::kBindingQuery:
      {
         // Yadoms ask for a binding query 
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
            std::cerr << errorMessage << std::endl;
         }
         break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());

         //if (!initializationError)
            m_ioManager->onCommand(api, command);
         break;
      }
      default:
         {
            std::cerr << "Unknown message id" << std::endl;
            break;
         }
      }
   }
}

void CIPX800::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   std::cout << "Update configuration..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}