#include "stdafx.h"
#include "WeatherUnderground.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/http/HttpMethods.h>
#include <shared/exception/Exception.hpp>
#include "ErrorAnswerHandler.h"
#include "RequestErrorException.hpp"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWeatherUnderground)

CWeatherUnderground::CWeatherUnderground()
   : m_deviceName("WeatherUnderground"),
     m_runningState(EWUPluginState::kUndefined)
{}

CWeatherUnderground::~CWeatherUnderground()
{}

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "CWeatherUnderground is starting..." << std::endl;

   int weatherConditionsSendingRetry = 0;
   int astronomySendingRetry = 0;
   int forecast10daysSendingRetry = 0;

   api->getEventHandler().createTimer(kEvtInitialization, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

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
            setPluginState(api, EWUPluginState::kStop);
            return;
         }
      case kEvtInitialization:
      {
         try
         {
            // Load configuration values (provided by database)
            m_configuration.initializeWith(api->getConfiguration());

            // Create all existing devices
            m_factory = boost::make_shared<CWUFactory>(api, m_configuration);

            weatherConditionsRequester = m_factory->getWeatherConditionsDevice();
            astronomyRequester = m_factory->getAstronomyDevice();
            forecast10Days = m_factory->getForecastDevice();

            setPluginState(api, EWUPluginState::kRunning);
         }
         catch (CRequestErrorException&)
         {
            setPluginState(api, EWUPluginState::kNoConnection);
            api->getEventHandler().createTimer(kEvtInitialization, shared::event::CEventTimer::kOneShot, boost::posix_time::minutes(1));
         }
         catch (std::exception& e)
         {
            std::cout << "exception : " << e.what() << std::endl;
            // Informs Yadoms about the plugin actual state
            setPluginState(api, EWUPluginState::kInitializationError);
         }
         break;
      }
      case kEvtTimerRefreshWeatherConditions:
         {
            try
            {
               shared::CDataContainer returnData = SendUrlRequest(api, weatherConditionsRequester->getUrl(), kEvtTimerRefreshWeatherConditions, weatherConditionsSendingRetry);
               weatherConditionsRequester->parse(api, returnData, m_configuration);
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
               astronomyRequester->parse(api, returnData, m_configuration);
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
               forecast10Days->parse(api, returnData, m_configuration);
            }
            catch (CRequestErrorException&)
            {}
         }
         break;
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            setPluginState(api, EWUPluginState::kupdateConfiguration);
            try {
               onUpdateConfiguration(api, api->getEventHandler().getEventData<shared::CDataContainer>());
               setPluginState(api, EWUPluginState::kRunning);
            }
            catch (std::exception&)
            {
               setPluginState(api, EWUPluginState::kInitializationError);
            }
            break;
         }
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

   // Initialize the plugin with new forecast stations
   if (m_factory)
   {
      m_factory->createDevice(api, m_configuration);

      weatherConditionsRequester = m_factory->getWeatherConditionsDevice();
      astronomyRequester = m_factory->getAstronomyDevice();
      forecast10Days = m_factory->getForecastDevice();
   }
   else
   {
      std::cerr << "Factory not yet initialize, please check Ethernet connection" << std::endl;
   }
}

void CWeatherUnderground::setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, EWUPluginState newState)
{
   if (m_runningState != newState)
   {
      switch (newState)
      {
      case EWUPluginState::kInitializationError:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");
         break;
      case EWUPluginState::kQueryNotFound:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "querynotfound");
         break;
      case EWUPluginState::kKeyNotFound:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "keynotfound");
         break;
      case EWUPluginState::kupdateConfiguration:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "updateconfiguration");
         break;
      case EWUPluginState::kNoConnection:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
         break;
      case EWUPluginState::kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case EWUPluginState::kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         break;
      }

      m_runningState = newState;
   }
}

shared::CDataContainer CWeatherUnderground::SendUrlRequest(boost::shared_ptr<yApi::IYPluginApi> api, std::string url, int event, int &nbRetry)
{
   try
   {
      shared::CDataContainer data = shared::CHttpMethods::SendGetRequest(url);

      ErrorAnswerHandler Response(api, data);

      if (Response.ContainError())
      {
         api->setPluginState(yApi::historization::EPluginState::kCustom, Response.getError());
         throw shared::exception::CException("Response contain error");
      }
      //All is ok we reinitialize the nbRetry
      nbRetry = 0;

      return data;
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << ". Retry in 1 minute." << std::endl;
      api->getEventHandler().createTimer(event, shared::event::CEventTimer::kOneShot, boost::posix_time::minutes(1));

      if (nbRetry == 3)
         ++nbRetry;
      else
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");

      throw CRequestErrorException();
   }
}
