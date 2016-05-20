#include "stdafx.h"
#include "SystemInformation.h"
#include <SystemFactory.h>
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CSystemInformation)


CSystemInformation::CSystemInformation()
   : m_deviceName("SystemInformation")
{
}

CSystemInformation::~CSystemInformation()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshDiskAndMemory
};

void CSystemInformation::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "SystemInformation is starting..." << std::endl;

   m_configuration.initializeWith(api->getConfiguration());

   // Device declaration, if needed
   if (!api->deviceExists(m_deviceName))
      api->declareDevice(m_deviceName, "SystemInformation");

   shared::CDataContainer details;
   details.set("provider", "SystemInformation");
   details.set("shortProvider", "si");

   CSystemFactory Factory(api, m_deviceName, m_configuration, details);

   // Event to be sent immediately for the first value
   api->getEventHandler().createTimer(kEvtTimerRefreshCPULoad, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   // Timer used to read periodically CPU loads
   api->getEventHandler().createTimer(kEvtTimerRefreshCPULoad, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(30));

   // Event to be sent immediately for the first value
   api->getEventHandler().createTimer(kEvtTimerRefreshDiskAndMemory, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
   // Timer used to read periodically Disk Usage and Memory Load
   api->getEventHandler().createTimer(kEvtTimerRefreshDiskAndMemory, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(300));

   // the main loop
   std::cout << "SystemInformation plugin is running..." << std::endl;

   while (1)
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
      case kEvtTimerRefreshCPULoad:
         {
            Factory.OnSpeedUpdate(api);
            break;
         }
      case kEvtTimerRefreshDiskAndMemory:
         {
            Factory.OnSlowUpdate(api, m_configuration);
            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            Factory.OnConfigurationUpdate(api, m_configuration, details);
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

void CSystemInformation::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   std::cout << "Update configuration..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}

