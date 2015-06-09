#include "stdafx.h"
#include "OneWire.h"
#include "Factory.h"
#include "Configuration.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(COneWire)


COneWire::COneWire()
   :m_configuration(new CConfiguration())
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

void COneWire::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "OneWire is starting...";

      m_configuration->initializeWith(context->getConfiguration());
      m_engine = CFactory::createEngine(context, m_configuration);

      // 1-wire Network devices
      std::map<std::string, boost::shared_ptr<device::IDevice> > devices;

      // Periodic network refresh
      context->getEventHandler().createTimer(kEvtTimerNetworkRefresh, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // the main loop
      YADOMS_LOG(debug) << "OneWire plugin is running...";

      while (1)
      {
         // Wait for an event
         switch (context->getEventHandler().waitForEvents())
         {
         case kEvtTimerNetworkRefresh:
         {
            // Scan 1-wire network for all devices
            std::map<std::string, boost::shared_ptr<device::IDevice> > foundDevices = m_engine->scanNetwork();

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
            boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
            onCommand(devices, command);

            break;
         }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            onUpdateConfiguration(context, context->getEventHandler().getEventData<shared::CDataContainer>());
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
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(information) << "OneWire is stopping..."  << std::endl;
   }

   m_engine.reset();
}

void COneWire::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData)
{
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Configuration was updated
   YADOMS_LOG(debug) << "Configuration was updated...";

   bool needToRestartEngine = m_engine->newConfigurationRequireRestart(newConfigurationData);

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);

   // If needed, rebuild engine
   if (needToRestartEngine)
      m_engine = CFactory::createEngine(context, m_configuration);
}

void COneWire::onCommand(std::map<std::string, boost::shared_ptr<device::IDevice> >& devices, boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   YADOMS_LOG(debug) << "Command received :" << command->toString();

   std::map<std::string, boost::shared_ptr<device::IDevice> >::iterator device = devices.find(command->getTargetDevice());
   if (device == devices.end())
   {
      YADOMS_LOG(warning) << "Device " << command->getTargetDevice() << "not found on the 1-wire network";
   }

   device->second->set(command->getKeyword(), command->getBody());
}

void COneWire::updateNetwork(std::map<std::string, boost::shared_ptr<device::IDevice> >& devices, const std::map<std::string, boost::shared_ptr<device::IDevice> >& foundDevices)
{
   for (std::map<std::string, boost::shared_ptr<device::IDevice> >::const_iterator foundDevice = foundDevices.begin(); foundDevice != foundDevices.end(); ++foundDevice)
   {
      if (devices.find(foundDevice->first) != devices.end())
      {
         // New device
         // First, add it to main list
         devices[foundDevice->first] = foundDevice->second;
         // Now, declare it to Yadoms
         foundDevice->second->declare();
      }
   }
}