#include "stdafx.h"
#include "Piface2.h"
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
      default:
         {
            std::cerr << "Unknown message id" << std::endl;
            break;
         }
      }
   }
}