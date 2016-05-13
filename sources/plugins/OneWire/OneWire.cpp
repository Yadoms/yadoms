#include "stdafx.h"
#include "OneWire.h"
#include "Factory.h"
#include "Configuration.h"
#include "OneWireException.hpp"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(COneWire)


COneWire::COneWire()
   :m_configuration(boost::make_shared<CConfiguration>())
{
}

COneWire::~COneWire()
{
}

// Event IDs
enum
{
   kEvtTimerNetworkRefresh = yApi::IYPluginApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
};

void COneWire::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      std::cout << "OneWire is starting..." << std::endl;

      m_configuration->initializeWith(api->getConfiguration());
      m_engine = CFactory::createEngine(api, m_configuration);

      // 1-wire Network devices
      std::map<std::string, boost::shared_ptr<device::IDevice> > devices;

      // Periodic network refresh
      api->getEventHandler().createTimer(kEvtTimerNetworkRefresh, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(5));

      // the main loop
      std::cout << "OneWire plugin is running..." << std::endl;

      while (1)
      {
         try
         {
            // Wait for an event
            switch (api->getEventHandler().waitForEvents())
            {
            case kEvtTimerNetworkRefresh:
            {
               // Scan 1-wire network for new devices and update our network image
               updateNetwork(devices, m_engine->scanNetwork());

               // Now read all devices state and historize data
               for (std::map<std::string, boost::shared_ptr<device::IDevice> >::const_iterator device = devices.begin(); device != devices.end(); ++device)
               {
                  // Set here an interruption point because it can take some time in case of big networks
                  boost::this_thread::interruption_point();
                  device->second->historize();
               }

               break;
            }
            case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // A command was received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
               onCommand(devices, command);

               break;
            }
            case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
               break;
            }
            default:
            {
               std::cerr << "Unknown message id" << std::endl;
               break;
            }
            }
         }
         catch (COneWireException& e)
         {
            std::cerr << e.what() << std::endl;
         }
      }
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      std::cout << "OneWire is stopping..."  << std::endl;
   }

   m_engine.reset();
}

void COneWire::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Configuration was updated
   std::cout << "Configuration was updated..." << std::endl;

   bool needToRestartEngine = m_engine->newConfigurationRequireRestart(newConfigurationData);

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);

   // If needed, rebuild engine
   if (needToRestartEngine)
      m_engine = CFactory::createEngine(api, m_configuration);
}

void COneWire::onCommand(std::map<std::string, boost::shared_ptr<device::IDevice> >& devices, boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   std::cout << "Command received :" << command->toString() << std::endl;

   std::map<std::string, boost::shared_ptr<device::IDevice> >::iterator device = devices.find(command->getTargetDevice());
   if (device == devices.end())
   {
      std::cout << "Device " << command->getTargetDevice() << " not found on the 1-wire network" << std::endl;
      return;
   }

   device->second->set(command->getKeyword(), command->getBody());
}

void COneWire::updateNetwork(std::map<std::string, boost::shared_ptr<device::IDevice> >& devices, const std::map<std::string, boost::shared_ptr<device::IDevice> >& foundDevices)
{
   for (std::map<std::string, boost::shared_ptr<device::IDevice> >::const_iterator foundDevice = foundDevices.begin(); foundDevice != foundDevices.end(); ++foundDevice)
   {
      if (devices.find(foundDevice->first) == devices.end())
      {
         // New device
         // First, add it to main list
         devices[foundDevice->first] = foundDevice->second;
         // Now, declare it to Yadoms
         foundDevice->second->declare();
      }
   }
}
