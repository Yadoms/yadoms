#include "stdafx.h"
#include "IPX800.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/http/HttpMethods.h>
#include "IPX800Factory.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CIPX800)


CIPX800::CIPX800()
   : m_deviceName("IPX800")
{
}

CIPX800::~CIPX800()
{
}

// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId // Always start from shared::event::CEventHandler::kUserFirstId
};

void CIPX800::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "IPX800 is starting..." << std::endl;
      
   try {
      m_configuration.initializeWith(api->getConfiguration());
   }
   catch (...)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
   }

   shared::CDataContainer details;
   details.set("provider", "IPX800");
   details.set("shortProvider", "ipx");

   //CIPX800Factory factory(api, m_deviceName, m_configuration, details);

   SendCommand(m_configuration.GetIPAddress(), m_configuration.GetPassword());

   // the main loop
   std::cout << "IPX800 plugin is running..." << std::endl;

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

void CIPX800::SendCommand(Poco::Net::IPAddress IPAddress, std::string M2MPassword)
{
   std::stringstream url;

   // create the URL
   url << "http://" << IPAddress.toString() << "/api/xdevices.json";// ? key = " << M2MPassword << "&SetR = 01";
   std::cout << url.str();

   shared::CDataContainer parameters;

   parameters.set("key", M2MPassword);
   parameters.set("SetR", "01");

   shared::CDataContainer data = shared::CHttpMethods::SendGetRequest(url.str(), parameters);

   try {
      auto returnValue = data.get<std::string>("Success");

      std::cout << "ok" << std::endl;
      std::cout << returnValue << std::endl;
   }
   catch (...)
   {
      std::cout << "not found" << std::endl;
   }
}