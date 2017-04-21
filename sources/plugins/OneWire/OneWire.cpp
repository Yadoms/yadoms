#include "stdafx.h"
#include "OneWire.h"
#include "Factory.h"
#include "Configuration.h"
#include "OneWireException.hpp"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>


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
   kEvtTimerNetworkRefresh = yApi::IYPluginApi::kPluginFirstEventId,
};

void COneWire::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   std::cout << "OneWire is starting..." << std::endl;

   m_configuration->initializeWith(api->getConfiguration());
   m_engine = CFactory::createEngine(api, m_configuration);

   // 1-wire Network devices
   std::map<std::string, boost::shared_ptr<device::IDevice> > devices;

   // Periodic network refresh
   api->getEventHandler().createTimer(kEvtTimerNetworkRefresh,
                                      shared::event::CEventTimer::kPeriodic,
                                      boost::posix_time::seconds(5));

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // the main loop
   std::cout << "OneWire plugin is running..." << std::endl;

   while (1)
   {
      try
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
         case kEvtTimerNetworkRefresh:
         {
            // Scan 1-wire network for new devices and update our network image
            updateNetwork(api, devices, m_engine->scanNetwork());

            // Now read all devices state and historize data
            for (auto device = devices.begin(); device != devices.end(); ++device)
            {
               // Set here an interruption point because it can take some time in case of big networks
               boost::this_thread::interruption_point();

               boost::shared_ptr<device::IDevice> newDevice = device->second;
               newDevice->read();
               if (!newDevice->keywords().empty())
                  api->historizeData(newDevice->ident()->deviceName(),
                                     newDevice->keywords());
            }

            break;
         }
         case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // A command was received from Yadoms
            auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();
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

void COneWire::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const shared::CDataContainer& newConfigurationData)
{
   BOOST_ASSERT(!newConfigurationData.empty());  // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Configuration was updated
   std::cout << "Configuration was updated..." << std::endl;

   auto needToRestartEngine = m_engine->newConfigurationRequireRestart(newConfigurationData);

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);

   // If needed, rebuild engine
   if (needToRestartEngine)
      m_engine = CFactory::createEngine(api, m_configuration);
}

void COneWire::onCommand(std::map<std::string,
   boost::shared_ptr<device::IDevice> >& devices,
                         boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;

   std::map<std::string, boost::shared_ptr<device::IDevice> >::iterator device = devices.find(command->getDevice());
   if (device == devices.end())
   {
      std::cout << "Device " << command->getDevice() << " not found on the 1-wire network" << std::endl;
      return;
   }

   device->second->write(command->getKeyword(), command->getBody());
}

void COneWire::updateNetwork(boost::shared_ptr<yApi::IYPluginApi> api,
                             std::map<std::string, boost::shared_ptr<device::IDevice> >& devices,
                             const std::map<std::string, boost::shared_ptr<device::IDevice> >& foundDevices)
{
   for (auto foundDevice = foundDevices.begin(); foundDevice != foundDevices.end(); ++foundDevice)
   {
      if (devices.find(foundDevice->first) == devices.end())
      {
         // New device
         boost::shared_ptr<device::IDevice> newDevice = foundDevice->second;
         // First, add it to main list
         devices[foundDevice->first] = newDevice;
         // Now, declare it to Yadoms
         if (!api->deviceExists(newDevice->ident()->deviceName()))
            api->declareDevice(newDevice->ident()->deviceName(),
                               newDevice->ident()->model(),
                               newDevice->ident()->model(),
                               newDevice->keywords(),
                               newDevice->ident()->details());
      }
   }
}
