#include "stdafx.h"
#include "OneWire.h"
#include "Factory.h"
#include "Configuration.h"
#include "OneWireException.hpp"
#include <shared/event/EventTimer.h>
#include <shared/exception/EmptyResult.hpp>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/Log.h>

IMPLEMENT_PLUGIN(COneWire)

COneWire::COneWire()
    : m_configuration(boost::make_shared<CConfiguration>())
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

   YADOMS_LOG(information) << "OneWire is starting...";

   m_configuration->initializeWith(api->getConfiguration());
   m_engine = CFactory::createEngine(api, m_configuration);

   // 1-wire Network devices
   std::map<std::string, boost::shared_ptr<device::IDevice>> devices;

   // Periodic network refresh
   api->getEventHandler().createTimer(kEvtTimerNetworkRefresh,
                                      shared::event::CEventTimer::kPeriodic,
                                      boost::posix_time::seconds(5));

   api->setPluginState(yApi::historization::EPluginState::kRunning);

   // the main loop
   YADOMS_LOG(information) << "OneWire plugin is running...";

   while (true)
   {
      try
      {
         // Wait for an event
         switch (api->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested";
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
         case kEvtTimerNetworkRefresh:
         {
            // Scan 1-wire network for new devices and update our network image
            updateNetwork(api, devices, m_engine->scanNetwork());
            YADOMS_LOG(trace) << "Netword updated, " << devices.size() << " device(s) found";

            // Now read all devices state and historize data
            for (const auto& device : devices)
            {
               // Set here an interruption point because it can take some time in case of big networks
               boost::this_thread::interruption_point();

               boost::shared_ptr<device::IDevice> newDevice = device.second;
               YADOMS_LOG(trace) << "  Read " << newDevice->ident()->deviceName() << " state...";
               newDevice->read();
               if (newDevice->hasRelevantValue())
                  api->historizeData(newDevice->ident()->deviceName(),
                                     newDevice->keywords());
            }

            break;
         }
         case yApi::IYPluginApi::kEventDeviceRemoved:
         {
            const auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved>>();
            if (devices.find(device->device()) == devices.end())
               YADOMS_LOG(error) << "Fail to remove " << device->device() << " device : device unknown";
            else
               devices.erase(device->device());
            YADOMS_LOG(information) << "Device " << device->device() << " removed";
            break;
         }
         case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // A command was received from Yadoms
            auto command = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>();
            onCommand(devices, command);

            break;
         }
         case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            onUpdateConfiguration(api, api->getEventHandler().getEventData<boost::shared_ptr<shared::CDataContainer>>());
            break;
         }
         case yApi::IYPluginApi::kSetDeviceConfiguration:
         {
            auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();
            onDeviceConfiguration(api,
                                  devices,
                                  deviceConfiguration);
            break;
         }
         default:
         {
            YADOMS_LOG(error) << "Unknown message id";
            break;
         }
         }
      }
      catch (COneWireException &e)
      {
         YADOMS_LOG(error) << e.what();
      }
   }
}

void COneWire::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const boost::shared_ptr<shared::CDataContainer> &newConfigurationData)
{
   BOOST_ASSERT(!newConfigurationData->empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Configuration was updated
   YADOMS_LOG(information) << "Configuration was updated...";

   auto needToRestartEngine = m_engine->newConfigurationRequireRestart(newConfigurationData);

   // Update configuration
   m_configuration->initializeWith(newConfigurationData);

   // If needed, rebuild engine
   if (needToRestartEngine)
      m_engine = CFactory::createEngine(api, m_configuration);
}

void COneWire::onDeviceConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                     std::map<std::string, boost::shared_ptr<device::IDevice>> &devices,
                                     boost::shared_ptr<const yApi::ISetDeviceConfiguration> deviceConfiguration)
{
   auto device = devices.find(deviceConfiguration->name());
   if (device == devices.end())
   {
      YADOMS_LOG(error) << "Configurable device " << deviceConfiguration->name() << " not found";
      return;
   }

   device->second->setConfiguration(api,
                                    deviceConfiguration->configuration());
}

void COneWire::onCommand(std::map<std::string, boost::shared_ptr<device::IDevice>> &devices,
                         boost::shared_ptr<const yApi::IDeviceCommand> command)
{
   YADOMS_LOG(information) << "Command received :" << yApi::IDeviceCommand::toString(command);

   std::map<std::string, boost::shared_ptr<device::IDevice>>::iterator device = devices.find(command->getDevice());
   if (device == devices.end())
   {
      YADOMS_LOG(information) << "Device " << command->getDevice() << " not found on the 1-wire network";
      return;
   }

   device->second->write(command->getKeyword(), command->getBody());
}

void COneWire::updateNetwork(boost::shared_ptr<yApi::IYPluginApi> api,
                             std::map<std::string, boost::shared_ptr<device::IDevice>> &devices,
                             const std::map<std::string, boost::shared_ptr<device::IDevice>> &foundDevices)
{
   for (const auto &foundDevice : foundDevices)
   {
      if (devices.find(foundDevice.first) != devices.end())
         continue;

      YADOMS_LOG(information) << "New device found " << foundDevice.first;

      // New device
      boost::shared_ptr<device::IDevice> newDevice = foundDevice.second;
      // First, add it to main list
      devices[foundDevice.first] = newDevice;
      // Now, declare it to Yadoms
      if (!api->deviceExists(newDevice->ident()->deviceName()))
      {
         // Declare device
         api->declareDevice(newDevice->ident()->deviceName(),
                            newDevice->ident()->model(),
                            newDevice->ident()->model(),
                            newDevice->keywords(),
                            newDevice->ident()->details());

         // Ask device configuration to set keywords list
         try
         {
            const auto deviceConfiguration = api->getDeviceConfiguration(newDevice->ident()->deviceName());
            if (!deviceConfiguration->empty())
               newDevice->setConfiguration(api,
                                           deviceConfiguration);
         }
         catch (shared::exception::CEmptyResult &)
         {
         }
      }
   }
}
