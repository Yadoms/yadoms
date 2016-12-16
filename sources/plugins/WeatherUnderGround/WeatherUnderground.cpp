#include "stdafx.h"
#include "WeatherUnderground.h"
#include "deviceConfiguration.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>
#include <shared/plugin/yPluginApi/IDeviceRemoved.h>
#include "Features/WeatherConditions.h"
#include "Features/Astronomy.h"
#include "Features/ForecastDays.h"
#include <shared/http/HttpMethods.h>
#include <shared/exception/Exception.hpp>
#include "ErrorAnswerHandler.h"
#include "RequestErrorException.hpp"
#include "Features/LiveStations.h"

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

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "CWeatherUnderground is starting..." << std::endl;

   int weatherConditionsSendingRetry = 0;
   int astronomySendingRetry = 0;
   int forecast10daysSendingRetry = 0;

   boost::shared_ptr<CWeatherConditions> weatherConditionsRequester;
   boost::shared_ptr<CAstronomy> astronomyRequester;
   boost::shared_ptr<CForecastDays> forecast10Days;
   //std::vector<shared::CDataContainer> forecastStations;

   try
   {
      // Load configuration values (provided by database)
      m_configuration.initializeWith(api->getConfiguration());

      // Create all existing devices
      m_factory = boost::make_shared<CWUFactory>(api, m_configuration);
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
        auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
        std::cout << "Manually device creation request received for device :" << request->getData().getDeviceName() << std::endl;

        //Display the configuration
        request->getData().getConfiguration().printToLog(std::cout);

        try {
           // Device creation
           m_factory->createDeviceManually(api, m_configuration, request);
        }
        catch (std::exception& e)
        {
           request->sendError(e.what());
        }
		  break;
	  }
     case yApi::IYPluginApi::kSetDeviceConfiguration:
     {
        // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
        auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();

        m_factory->onDeviceSetConfiguration(api, deviceConfiguration->device(), m_configuration, deviceConfiguration->configuration());

        break;
     }
	  case yApi::IYPluginApi::kEventDeviceRemoved:
	  {
		  auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved> >();
		  std::cout << device->device() << " was removed" << std::endl;

        m_factory->removeDevice(api, device->device());

		  break;
	  }
	  case yApi::IYPluginApi::kBindingQuery:
	  {
		  // Yadoms ask for a binding query 
        auto data = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IBindingQueryRequest> >();

		  try {
			if (data->getData().getQuery() == "LiveStations")
			{
				data->sendSuccess(m_factory->bindAvailableStations());
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
               shared::CDataContainer returnData = SendUrlRequest(api, weatherConditionsRequester->getUrl(), kEvtTimerRefreshWeatherConditions, weatherConditionsSendingRetry);
               weatherConditionsRequester->parse(api, returnData);
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
               shared::CDataContainer returnData = SendUrlRequest(api, astronomyRequester->getUrl(), kEvtTimerRefreshAstronomy, astronomySendingRetry);
               astronomyRequester->parse(api, returnData);
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
               shared::CDataContainer returnData = SendUrlRequest(api, forecast10Days->getUrl(), kEvtTimerRefreshForecast10Days, forecast10daysSendingRetry);
               forecast10Days->parse(api, returnData);
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

            // TODO : Update Live Station if any -> Enter the new location, for example from the plugin configuration
            // Le faire par la factory
            // Virer les locations ci-dessous.
            //boost::shared_ptr<CLiveStations> newLiveStation();
            boost::shared_ptr<const shared::ILocation> location;

            // Update configurations
            if (weatherConditionsRequester) weatherConditionsRequester->onPluginUpdate(api, m_configuration, location);
            if (astronomyRequester) astronomyRequester->onPluginUpdate(api, m_configuration, location);
            if (forecast10Days) forecast10Days->onPluginUpdate(api, m_configuration, location);
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