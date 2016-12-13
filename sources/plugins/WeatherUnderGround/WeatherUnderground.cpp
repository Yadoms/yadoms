#include "stdafx.h"
#include "WeatherUnderground.h"
#include "deviceConfiguration.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>
#include "WeatherConditions.h"
#include "Astronomy.h"
#include "ForecastDays.h"
#include <shared/http/HttpMethods.h>
#include <shared/exception/Exception.hpp>
#include "ErrorAnswerHandler.h"
#include "RequestErrorException.hpp"
#include "LiveStations.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWeatherUnderground)


CWeatherUnderground::CWeatherUnderground()
   : m_deviceName("WeatherUnderground"),
     m_runningState(false)
{}

CWeatherUnderground::~CWeatherUnderground()
{}

// Event IDs
enum
{
   kEvtTimerRefreshWeatherConditions = yApi::IYPluginApi::kPluginFirstEventId, // Always start from shared::event::CEventHandler::kUserFirstId
   kEvtTimerRefreshAstronomy,
   kEvtTimerRefreshForecast10Days,
   kEvtPluginState
};

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "CWeatherUnderground is starting..." << std::endl;

   int weatherConditionsSendingRetry = 0;
   int astronomySendingRetry = 0;
   int forecast10daysSendingRetry = 0;

   bool weatherConditionsFinished = false;
   bool astronomyFinished = false;
   bool forecast10daysFinished = false;

   boost::shared_ptr<CWeatherConditions> weatherConditionsRequester;
   boost::shared_ptr<CAstronomy> astronomyRequester;
   boost::shared_ptr<CForecastDays> forecast10Days;
   std::vector<shared::CDataContainer> forecastStations;

   try
   {
      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());

      //Get all forecast stations to be displayed into the menu
      CLiveStations liveStations(api);
      liveStations.getAllStations(api, m_configuration.getAPIKey());
   }
   catch (...)
   {
      // Informs Yadoms about the plugin actual state
      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
   }

   std::cout << "CWeatherUnderground plugin is running..." << std::endl;

   // the main loop
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
	  case yApi::IYPluginApi::kEventManuallyDeviceCreation:
	  {
		  // Yadoms asks for device creation
		  //TODO : A remplir !
		  auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
		  std::cout << "Manually device creation request received for device :" << request->getData().getDeviceName() << std::endl;

        //Display the configuration
        request->getData().getConfiguration().printToLog(std::cout);

        boost::shared_ptr<IdeviceConfiguration> deviceConfiguration = boost::make_shared<CdeviceConfiguration>(request->getData().getConfiguration());

        // If astronomy is enabled
        if (deviceConfiguration->isAstronomyEnabled())
        {
           astronomyRequester = boost::make_shared<CAstronomy>(api, m_configuration, deviceConfiguration);

           api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

           // TODO : Voir pour ne pas créer plusieurs fois ce timer
           api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));
        }

        // if conditions are enabled
        if (deviceConfiguration->isConditionsIndividualKeywordsEnabled() || deviceConfiguration->isLiveConditionsEnabled())
        {
           weatherConditionsRequester = boost::make_shared<CWeatherConditions>(api, m_configuration, deviceConfiguration);
           api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

           // TODO : Voir pour ne pas créer plusieurs fois ce timer
           api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));
        }

        // If forecast is enabled
        if (deviceConfiguration->isForecast10DaysEnabled())
        {
           forecast10Days = boost::make_shared<CForecastDays>(api, m_configuration, deviceConfiguration);
           api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

           // TODO : Voir pour ne pas créer plusieurs fois ce timer
           api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));
        }

		  break;
	  }
	  case yApi::IYPluginApi::kEventDeviceRemoved:
	  {
          // TODO : A remplir !
		  auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
		  std::cout << device->device() << " was removed" << std::endl;
		  break;
	  }
	  case yApi::IYPluginApi::kBindingQuery:
	  {
		  // Yadoms ask for a binding query 
        auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest> >();

		  try {
			if (data->getData().getQuery() == "LiveStations")
			{
				// Create the station list
				std::vector<shared::CDataContainer>::const_iterator iterStations;
            int counter = 1;
            shared::CDataContainer value;
            shared::CDataContainer stations;

            try {
               for (iterStations = forecastStations.begin(); iterStations != forecastStations.end(); ++iterStations)
               {
                  std::string city = (*iterStations).get<std::string>("city");

                  // Only filled elements are copied
                  if (city != "")
                     stations.set(boost::lexical_cast<std::string>(counter + 1), city + " " + (*iterStations).get<std::string>("country"));

                  ++counter;

                  /* return format :
                  -city :
                  -state :
                  -country :
                  -icao :
                  -lat :
                  -lon :
                  */
               }
            }
            catch (std::exception& e)
            {
               std::cout << "exception " << e.what() << std::endl;
               std::cout << "No Stations return in binding" << std::endl;

               // reset values
               shared::CDataContainer defaultReturn;
               stations.initializeWith(defaultReturn);
            }

            value.set("required", "true");
            value.set("type", "enum");
            value.set("values", stations);
            value.set("defaultValue", "1");

				data->sendSuccess(value);
			}
			else
			{
				std::string errorMessage = (boost::format("unknown query : %1%") % data->getData().getQuery()).str();
				data->sendError(errorMessage);
				std::cerr << errorMessage << std::endl;
			}
		  }
		  catch (std::exception& e)
		  {
           std::string errorMessage("Unknow error : ");
           errorMessage += e.what();
           std::cout << errorMessage;
           data->sendError(errorMessage);
		  }
		  break;
	  }
      case kEvtTimerRefreshWeatherConditions:
         {
            try
            {
               weatherConditionsFinished = false;
               shared::CDataContainer returnData = SendUrlRequest(api, weatherConditionsRequester->getUrl(), kEvtTimerRefreshWeatherConditions, weatherConditionsSendingRetry);
               weatherConditionsRequester->parse(api, returnData);
               weatherConditionsFinished = true;
               api->getEventHandler().createTimer(kEvtPluginState, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            }
            catch(CRequestErrorException& )
            {}
         }
         break;
      case kEvtTimerRefreshAstronomy:
         {
            try
            {
               astronomyFinished = false;
               shared::CDataContainer returnData = SendUrlRequest(api, astronomyRequester->getUrl(), kEvtTimerRefreshAstronomy, astronomySendingRetry);
               astronomyRequester->parse(api, returnData);
               astronomyFinished = true;
               api->getEventHandler().createTimer(kEvtPluginState, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            }
            catch (CRequestErrorException&)
            {}
         }
         break;
      case kEvtTimerRefreshForecast10Days:
         {
            try
            {
               // TODO : A modifier !
               if (!weatherConditionsFinished)
                  forecast10Days->setCityName(m_configuration.getLocalisation());
               else
                  forecast10Days->setCityName(weatherConditionsRequester->getCityName());

               forecast10daysFinished = false;

               shared::CDataContainer returnData = SendUrlRequest(api, forecast10Days->getUrl(), kEvtTimerRefreshForecast10Days, forecast10daysSendingRetry);
               forecast10Days->parse(api, returnData);
               forecast10daysFinished = true;
               api->getEventHandler().createTimer(kEvtPluginState, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
            }
            catch (CRequestErrorException&)
            {}
         }
         break;
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            shared::CDataContainer returnData;
            onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());

            // Update configurations
            weatherConditionsRequester->onPluginUpdate(api, m_configuration);
            astronomyRequester->onPluginUpdate(api, m_configuration);
            forecast10Days->onPluginUpdate(api, m_configuration);
/*
            if (m_configuration.IsConditionsIndividualKeywordsEnabled() || m_configuration.IsLiveConditionsEnabled())
               api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

            if (m_configuration.IsAstronomyEnabled())
               api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

            if (m_configuration.IsForecast10DaysEnabled())
               api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));
*/
            break;
         }
      case kEvtPluginState:
         {
/*
            // TODO : Voi comment estimer correctement l'état du plugin
            // estimate the state of the plugin
            if ((forecast10daysFinished || !m_configuration.isForecast10DaysEnabled()) &&
                (astronomyFinished || !m_configuration.isAstronomyEnabled()) &&
                weatherConditionsFinished || (!m_configuration.isConditionsIndividualKeywordsEnabled() && !m_configuration.isLiveConditionsEnabled()))
               newState = yApi::historization::EPluginState::kRunning;

            if (m_runningState != newState)
            {
               m_runningState = newState;

               if (m_runningState == yApi::historization::EPluginState::kRunning)
                  api->setPluginState(yApi::historization::EPluginState::kRunning);
            }
*/
         }
         break;
      default:
         std::cerr << "Unknown message id" << std::endl;
         break;
      }
   }
}

void CWeatherUnderground::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   std::cout << "Update configuration..." << std::endl;
   BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

   // Update configuration
   m_configuration.initializeWith(newConfigurationData);
}

shared::CDataContainer CWeatherUnderground::SendUrlRequest(boost::shared_ptr<yApi::IYPluginApi> api, std::string url, int event, int &nbRetry)
{
   try
   {
      shared::CDataContainer data = shared::CHttpMethods::SendGetRequest(url);

      ErrorAnswerHandler Response(api, data);

      if (Response.ContainError())
      {
         m_runningState = yApi::historization::EPluginState::kCustom;
         api->setPluginState(m_runningState, Response.getError());
         throw shared::exception::CException("Response contain error");
      }
      //All is ok we reinitialize the nbRetry
      nbRetry = 0;

      return data;
   }
   catch (shared::exception::CException& e)
   {
      if (nbRetry < 2)
      {
         std::cout << e.what() << ". Retry in 1 minute." << std::endl;
         api->getEventHandler().createTimer(event, shared::event::CEventTimer::kOneShot, boost::posix_time::minutes(1));
         ++nbRetry;
      }
      else
      {
         std::cout << e.what() << ". Stop retry." << std::endl;
         m_runningState = yApi::historization::EPluginState::kCustom;
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection"); 
         nbRetry = 0;
      }

      throw CRequestErrorException();
   }
}