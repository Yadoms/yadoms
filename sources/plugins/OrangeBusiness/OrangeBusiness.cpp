#include "stdafx.h"
#include "OrangeBusiness.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/Log.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(COrangeBusiness)

COrangeBusiness::COrangeBusiness()
{}

COrangeBusiness::~COrangeBusiness()
{}

//TODO : To be renamed ...
// Event IDs
enum
{
   kEvtTimerRefreshCPULoad = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kRefreshStatesReceived,
   kConnectionRetryTimer,
   kAnswerTimeout
};

void COrangeBusiness::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Orange Business plugin is starting...";

   m_configuration.initializeWith(api->getConfiguration());
      
   try {

      m_isDeveloperMode = api->getYadomsInformation()->developperMode();

      api->setPluginState(yApi::historization::EPluginState::kRunning);
      YADOMS_LOG(information) << "Orange Business plugin is running..." ;
   }
   catch (...)
   {
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
      YADOMS_LOG(error) << "Orange Business plugin initialization error..." ;
   }

   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
      {
         try {
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
            api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
         }
         catch (...)
         {
            YADOMS_LOG(information) << "Wrong configuration update";
         }
         break;
      }
      case yApi::IYPluginApi::kEventExtraQuery:
      {
         // Command was received from Yadoms
         auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery> >();
         if (extraQuery)
         {
            if (extraQuery->getData()->query() == "retreiveData")
            {
               processRetreiveData(api);
            }
         }
         extraQuery->sendSuccess(shared::CDataContainer());
         break;
      }
      case yApi::IYPluginApi::kEventDeviceCommand:
      {
         // Command received from Yadoms
         break;
      }
      default:
         {
            YADOMS_LOG(error) << "Unknown message id" ;
            break;
         }
      }
   }
}

void COrangeBusiness::processRetreiveData(boost::shared_ptr<yApi::IYPluginApi> api)
{
   shared::CDataContainer response = m_frameManager.getAllregisteredEquipments(m_configuration.getAPIKey()); //http://liveobjects.orange-business.com
   response.printToLog(YADOMS_LOG(information));
}

void COrangeBusiness::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}

void COrangeBusiness::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceName) const
{
   // keywords list for declaration
   //std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsDeclaration({ m_messageKeyword, m_rssi, m_batteryLevel, m_snr, m_signalPower });

   if (api->deviceExists(deviceName))
      return;

   std::string m_model = "";
   //api->declareDevice(deviceName, deviceName, m_model, m_keywordsDeclaration);
}