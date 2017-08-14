#include "stdafx.h"
#include "Piface2.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include "eventDefinitions.h"
#include "InitializationException.hpp"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CPiface2)

CPiface2::CPiface2()
   : m_deviceName("Piface2")
{
}

CPiface2::~CPiface2()
{
}

void CPiface2::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "Piface2 is starting..." << std::endl;

   bool initializationError = false;
   
   m_configuration.initializeWith(api->getConfiguration());

   shared::CDataContainer details;
   details.set("provider", "PiFace2");
   details.set("shortProvider", "pf2");

   try 
   {
      m_factory = boost::make_shared<CPiface2Factory>(api, m_deviceName, m_configuration, details);
      m_ioManager = m_factory->getIOManager();
   }
   catch (const CInitializationException& e)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
      std::cerr << e.what() << std::endl;
      initializationError = true;
   }
   catch (const CSPIException& e)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "SPIError");
      std::cerr << e.what() << std::endl;
      initializationError = true;
   }

   // the main loop
   std::cout << "Piface2 plugin is running..." << std::endl;

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
      case kEvtIOStateReceived:
      {
         //Value received from DI
		 if (!initializationError)
            m_ioManager->onCommand(api, api->getEventHandler().getEventData<const int>() );
         break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());

		 if (!initializationError)
            m_ioManager->onCommand(api, command);
         break;
      }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>(), details);
            api->setPluginState(yApi::historization::EPluginState::kRunning);
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

void CPiface2::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData, shared::CDataContainer& details)
{
   // Configuration was updated
   std::cout << "Update configuration..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);   
   
   // Update the configuration for sub-components
   m_factory->OnConfigurationUpdate(api, m_configuration, details);
}
