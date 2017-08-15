#include "stdafx.h"
#include "Piface2.h"
#include <shared/event/EventTimer.h>
#include <shared/Log.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include "eventDefinitions.h"
#include "InitializationException.hpp"
#include "SPIException.hpp"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CPiface2)

CPiface2::CPiface2()
   : m_deviceName("Piface2"),
     m_pluginState(EPiface2PluginState::kUndefined)
{
}

CPiface2::~CPiface2()
{
}

void CPiface2::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Piface2 is starting..." ;

   bool initializationError = false;
   
   m_configuration.initializeWith(api->getConfiguration());

   shared::CDataContainer details;
   details.set("provider", "PiFace2");
   details.set("shortProvider", "pf2");

   try 
   {
      m_factory = boost::make_shared<CPiface2Factory>(api, m_deviceName, m_configuration, details);
      m_ioManager = m_factory->getIOManager();
      setPluginState(api, EPiface2PluginState::kRunning);
   }
   catch (const CSPIException& e)
   {
      YADOMS_LOG(error) << e.what();
      initializationError = true;
      setPluginState(api, EPiface2PluginState::kSPIError);
   }
   catch (const CInitializationException& e)
   {
      YADOMS_LOG(error) << e.what();
      initializationError = true;
      setPluginState(api, EPiface2PluginState::kInitializationError);
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Unknow error";
      initializationError = true;
   }

   // the main loop
   YADOMS_LOG(information) << "Piface2 plugin is running..." ;

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            setPluginState(api, EPiface2PluginState::kStop);
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
            if (!initializationError) 
            {
               setPluginState(api, EPiface2PluginState::kupdateConfiguration);
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>(), details);
               setPluginState(api, EPiface2PluginState::kRunning);
            }
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

void CPiface2::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData, shared::CDataContainer& details)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);   
   
   // Update the configuration for sub-components
   m_factory->OnConfigurationUpdate(api, m_configuration, details);
}

void CPiface2::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, EPiface2PluginState newState)
{
   if (m_pluginState != newState)
   {
      switch (newState)
      {
      case kSPIError:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "SPIError");
         break;
      case kInitializationError:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
         break;
      case kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
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
