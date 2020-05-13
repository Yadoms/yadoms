#include "stdafx.h"
#include "OrangeBusiness.h"
#include "Decoder.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include <shared/Log.h>
#include <shared/http/HttpException.hpp>
#include "UnauthorizedException.hpp"

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
   kEvtTimerRefreshDevices = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kRefreshStatesReceived,
   kConnectionRetryTimer,
   kAnswerTimeout
};

void COrangeBusiness::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "Orange Business plugin is starting...";

   m_configuration.initializeWith(api->getConfiguration());
      
   try {
      m_frameManager = boost::make_shared<urlManager>();
      m_decoder = boost::make_shared<CDecoder>();
	   m_equipmentManager = boost::make_shared<CEquipmentManager>(api);

      if (m_equipmentManager->size() > 0)
      {
		   // read values of devices every 15mn.
         m_waitForAnswerTimer = api->getEventHandler().createTimer(kEvtTimerRefreshDevices,
                                                                   shared::event::CEventTimer::kPeriodic,
                                                                   boost::posix_time::minutes(15));

		   // fire immediately a event to read devices values
		   m_waitForAnswerTimer = api->getEventHandler().createTimer(kEvtTimerRefreshDevices,
			  													                   shared::event::CEventTimer::kOneShot,
																                   boost::posix_time::seconds(0));

         api->setPluginState(yApi::historization::EPluginState::kRunning);
         YADOMS_LOG(trace) << "State Running : device(s) registered";
      }
      else
      {  // If no equipment state is ready
         api->setPluginState(yApi::historization::EPluginState::kCustom, "ready");
         YADOMS_LOG(trace) << "State Ready : no device registered";
      }

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
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainerSharedPtr>());
            if (m_equipmentManager->size() > 0)
               api->setPluginState(yApi::historization::EPluginState::kRunning);
            else
               api->setPluginState(yApi::historization::EPluginState::kCustom, "ready");
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Wrong configuration update : " << e.what();
         }
         break;
      }
      case kEvtTimerRefreshDevices:
      {
		  try {
           if (m_equipmentManager)
           {
              try {
                 m_equipmentManager->refreshEquipments(api, m_frameManager, m_configuration.getAPIKey(), m_decoder);
              }
              catch (shared::CHttpException &)
              {
                 api->setPluginState(yApi::historization::EPluginState::kCustom, "noConnection");
                 YADOMS_LOG(error) << "Error during connection to the web site";
              }
              catch (std::exception &e)
              {
                 YADOMS_LOG(information) << "Error during equipment refresh : " << e.what();
              }
           }
		  }
		  catch (std::exception &e)
		  {
			  YADOMS_LOG(error) << "exception 3 : " << e.what();
		  }
		  break;
      }
      case yApi::IYPluginApi::kEventExtraQuery:
      {
         // Command was received from Yadoms
         auto extraQuery = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IExtraQuery> >();
         if (extraQuery)
         {
            try {
               if (extraQuery->getData()->query() == "retreiveData")
               {
                  registerAllDevices(api);

                  // fire immediately a event to read devices values
                  m_waitForAnswerTimer = api->getEventHandler().createTimer(kEvtTimerRefreshDevices,
                                                                            shared::event::CEventTimer::kOneShot,
                                                                            boost::posix_time::seconds(0));

                  api->setPluginState(yApi::historization::EPluginState::kRunning);
               }
               else if (extraQuery->getData()->query() == "onlyActivated")
               {
                  registerActivatedDevices(api);

                  // fire immediately a event to read devices values
                  m_waitForAnswerTimer = api->getEventHandler().createTimer(kEvtTimerRefreshDevices,
                                                                            shared::event::CEventTimer::kOneShot,
                                                                            boost::posix_time::seconds(0));

                  api->setPluginState(yApi::historization::EPluginState::kRunning);
               }
               else if (extraQuery->getData()->query() == "removeAllDevices")
               {
                  m_equipmentManager->removeAllDevices(api);
                  api->setPluginState(yApi::historization::EPluginState::kCustom, "ready"); // No more devices, so ready
               }

               extraQuery->sendSuccess(new_CDataContainerSharedPtr());
            }
            catch (shared::CHttpException &e)
            {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "noConnection");
               YADOMS_LOG(error) << "Error during connection to the web site";
               extraQuery->sendError(e.what());
            }
            catch (CUnauthorizedException &e)
            {
               api->setPluginState(yApi::historization::EPluginState::kCustom, "unauthorizedAccess");
               YADOMS_LOG(error) << e.what();
               extraQuery->sendError(e.what());
            }
            catch (std::exception &e)
            {
               YADOMS_LOG(information) << "Error during extra query : " << e.what();
               extraQuery->sendError(e.what());
            }
         }
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
   int page = 0;
   shared::CDataContainerSharedPtr response;

   do {
      response = m_frameManager->getRegisteredEquipments(m_configuration.getAPIKey(), page, false); //http://liveobjects.orange-business.com
      m_equipmentManager = boost::make_shared<CEquipmentManager>(m_decoder->decodeDevicesMessage(api, response));
      response->printToLog(YADOMS_LOG(information));
      ++page;
   } while (!m_decoder->isFrameComplete(response));
}

void COrangeBusiness::registerActivatedDevices(boost::shared_ptr<yApi::IYPluginApi> api)
{
   int page = 0;
   shared::CDataContainerSharedPtr response;

   do {
      response = m_frameManager->getRegisteredEquipments(m_configuration.getAPIKey(), page, true); //http://liveobjects.orange-business.com
      m_decoder->decodeDevicesMessage(api, response);
      response->printToLog(YADOMS_LOG(information));
      ++page;
   } while (!m_decoder->isFrameComplete(response));;
}

void COrangeBusiness::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainerSharedPtr& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration...";
   BOOST_ASSERT(!newConfigurationData->empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}