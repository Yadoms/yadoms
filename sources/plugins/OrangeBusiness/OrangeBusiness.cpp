#include "stdafx.h"
#include "OrangeBusiness.h"
#include "Decoder.h"
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

      m_decoder = boost::make_shared<CDecoder>(api);

      if (m_decoder->getDevices().size() > 0)
      {
         m_waitForAnswerTimer = api->getEventHandler().createTimer(kAnswerTimeout, // TODO : rename many things
                                                                   shared::event::CEventTimer::kOneShot,
                                                                   boost::posix_time::minutes(15));
      }

      // TODO : create others plugin state : ready, if no devices exists, for example, no connexion to the server, ...
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
      case kRefreshStatesReceived:
      {
         try {
            std::vector<boost::shared_ptr<equipments::IEquipment>> m_equipmentList = m_decoder->getDevices();
            std::vector<boost::shared_ptr<equipments::IEquipment>>::iterator iteratorEquipment;

            for (iteratorEquipment = m_equipmentList.begin(); iteratorEquipment != m_equipmentList.end(); ++iteratorEquipment)
            {
               int page = 0;

               try {
                  // reading of the battery level
                  shared::CDataContainer response = m_frameManager.getDeviceInformation(m_configuration.getAPIKey(), (*iteratorEquipment)->getEUI());
                  response.printToLog(YADOMS_LOG(information));
                  int batteryLevel = response.get<int>("lastBatteryLevel");

                  // Todo : Reading of the last communication date. If the date is too old for battery level > 1 week - do not integrate it

                  //Todo : Enter a date to limit the number of frames
                  // Reading all commands for an equipment. Go to the last page if necessary
                  do {
                     response = m_frameManager.listDeviceCommands(m_configuration.getAPIKey(), (*iteratorEquipment)->getEUI(), page);
                     response.printToLog(YADOMS_LOG(information));
                     ++page;
                  } while (!m_decoder->isFrameComplete(response));

                  // get last data from this last frame
                  m_decoder->getLastData(response);

                  // Todo : Reading of the last communication date. If the date is too old for data > 15mn - do not integrate it

               }
               catch (std::exception &e)
               {
                  YADOMS_LOG(error) << "exception : " << e.what();
               }
            }
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(error) << "exception : " << e.what();
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
               registerAllDevices(api); 
            } else if (extraQuery->getData()->query() == "onlyActivated")
            {
               registerActivatedDevices(api); 
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

void COrangeBusiness::registerAllDevices(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      int page = 0;
      shared::CDataContainer response;

      do {
         response = m_frameManager.getRegisteredEquipments(m_configuration.getAPIKey(), page, false); //http://liveobjects.orange-business.com
         m_decoder->decodeDevicesMessage(api, response);
         response.printToLog(YADOMS_LOG(information));
         ++page;
      } while (!m_decoder->isFrameComplete(response));
   }
   catch (std::exception &e)
   {
      YADOMS_LOG(error) << "exception : " << e.what();
   }
}

void COrangeBusiness::registerActivatedDevices(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      int page = 0;
      shared::CDataContainer response;

      do {
         response = m_frameManager.getRegisteredEquipments(m_configuration.getAPIKey(), page, true); //http://liveobjects.orange-business.com
         m_decoder->decodeDevicesMessage(api, response);
         response.printToLog(YADOMS_LOG(information));
         ++page;
      } while (!m_decoder->isFrameComplete(response));;
   }
   catch (std::exception &e)
   {
      YADOMS_LOG(error) << "exception : " << e.what();
   }
}

void COrangeBusiness::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}