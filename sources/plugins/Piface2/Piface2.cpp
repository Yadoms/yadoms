#include "stdafx.h"
#include "Piface2.h"
#include "Piface2Factory.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>

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

   shared::CDataContainer details;
   details.set("provider", "PiFace2");
   details.set("shortProvider", "pf2");

   CPiface2Factory m_factory(api, m_deviceName, details, kEvtIOStateReceived);

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
         std::cout << "Event received" << std::endl;
         api->historizeData(m_deviceName, api->getEventHandler().getEventData<const CIOState>().keyword);
         break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());

         std::cout << "Command received :" << yApi::IDeviceCommand::toString(command) << std::endl;
         
         //onCommand(api, command);

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