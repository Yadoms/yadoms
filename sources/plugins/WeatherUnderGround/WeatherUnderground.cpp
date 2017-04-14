#include "stdafx.h"
#include "WeatherUnderground.h"
#include <shared/event/EventTimer.h>
#include <plugin_cpp_api/ImplementationHelper.h>
#include <shared/http/HttpMethods.h>
#include <shared/exception/Exception.hpp>
#include "ErrorAnswerHandler.h"
#include "RequestErrorException.hpp"
#include "noLocationException.hpp"
#include "webSiteErrorException.hpp"
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>
#include <shared/Log.h>

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWeatherUnderground)

CWeatherUnderground::CWeatherUnderground()
   : m_deviceName("WeatherUnderground"),
     m_runningState(kUndefined)
{
}

CWeatherUnderground::~CWeatherUnderground()
{
}

void CWeatherUnderground::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   YADOMS_LOG(information) << "CWeatherUnderground is starting..." ;

   auto weatherConditionsSendingRetry = 0;
   auto astronomySendingRetry = 0;
   auto forecast10daysSendingRetry = 0;

   api->getEventHandler().createTimer(kEvtInitialization, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

   YADOMS_LOG(information) << "CWeatherUnderground plugin is running..." ;

   // the main loop
   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            YADOMS_LOG(information) << "Stop requested" ;
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
         catch (CNoLocationException&)
         { 
            YADOMS_LOG(error) << "No location configured" ;
            setPluginState(api, EWUPluginState::kNoLocation);
         }
         catch (CWebSiteErrorException& e)
         {
            YADOMS_LOG(information) << e.what() ;
            YADOMS_LOG(information) << "result :" << strcmp(e.what(), "keynotfound") ;

            if (boost::icontains(e.what(), "keynotfound"  )) setPluginState(api, EWUPluginState::kKeyNotFound);
            if (boost::icontains(e.what(), "querynotfound")) setPluginState(api, EWUPluginState::kQueryNotFound);
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(information) << "exception : " << e.what() ;
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
               setPluginState(api, EWUPluginState::kRunning);
            }
            catch (CRequestErrorException&)
            {}
			catch (std::exception& e)
			{
				YADOMS_LOG(error) << "error during weather refresh :" << e.what() ;
			}
         }
         break;
      case kEvtTimerRefreshAstronomy:
         {
            try
            {
               shared::CDataContainer returnData = SendUrlRequest(api, astronomyRequester->getUrl(), kEvtTimerRefreshAstronomy, astronomySendingRetry);
               astronomyRequester->parse(api, returnData, m_configuration);
               setPluginState(api, EWUPluginState::kRunning);
            }
            catch (CRequestErrorException&)
            {}
			catch (std::exception& e)
			{
				YADOMS_LOG(error) << "error during astronomy refresh :" << e.what() ;
			}
         }
         break;
      case kEvtTimerRefreshForecast10Days:
         {
            try
            {
               shared::CDataContainer returnData = SendUrlRequest(api, forecast10Days->getUrl(), kEvtTimerRefreshForecast10Days, forecast10daysSendingRetry);
               forecast10Days->parse(api, returnData, m_configuration);
               setPluginState(api, EWUPluginState::kRunning);
            }
            catch (CRequestErrorException&)
            {}
			catch (std::exception& e)
			{
				YADOMS_LOG(error) << "error during forecast refresh :" << e.what() ;
			}
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
      case yApi::IYPluginApi::kSetDeviceConfiguration:
      {
         // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
         auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();
         YADOMS_LOG(information) << "Set device configuration received, but not used" ;
         break;
      }
      default:
         YADOMS_LOG(warning) << "Unknown message id " << api->getEventHandler().getEventId();
         break;
      }
   }
}

void CWeatherUnderground::onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                                const shared::CDataContainer& newConfigurationData)
{
   // Configuration was updated
   YADOMS_LOG(information) << "Update configuration..." ;
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
      YADOMS_LOG(error) << "Factory not yet initialize, please check Ethernet connection" ;
      api->getEventHandler().createTimer(kEvtInitialization, shared::event::CEventTimer::kOneShot, boost::posix_time::minutes(1));
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
      case EWUPluginState::kNoLocation:
         api->setPluginState(yApi::historization::EPluginState::kCustom, "noLocation");
         break;
      case EWUPluginState::kRunning:
         api->setPluginState(yApi::historization::EPluginState::kRunning);
         break;
      case EWUPluginState::kStop:
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         break;
      default:
         YADOMS_LOG(error) << "this plugin status does not exist : " << newState ;
         break;
      }

      m_runningState = newState;
   }
}

shared::CDataContainer CWeatherUnderground::SendUrlRequest(boost::shared_ptr<yApi::IYPluginApi> api, const std::string url, const int event, int &nbRetry)
{
   try
   {
      shared::CDataContainer returnData;
      shared::CDataContainer noParameters;

      shared::CHttpMethods::SendGetRequest(url,
                                           noParameters,
                                           [&](shared::CDataContainer& data)
                                           {
                                              ErrorAnswerHandler Response(api, data);

                                              if (Response.ContainError())
                                                 throw CWebSiteErrorException(Response.getError());

                                              //All is ok we reinitialize the nbRetry
                                              nbRetry = 0;

                                              returnData = data;
                                           },
                                           httpRequestBindingTimeout);

      return returnData;
   }
   catch (CWebSiteErrorException& e)
   {
      if (boost::icontains(e.what(), "keynotfound")) setPluginState(api, EWUPluginState::kKeyNotFound);
      if (boost::icontains(e.what(), "querynotfound")) setPluginState(api, EWUPluginState::kQueryNotFound);

      throw CRequestErrorException();
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(information) << e.what() << ". Retry in 1 minute." ;
      api->getEventHandler().createTimer(event, shared::event::CEventTimer::kOneShot, boost::posix_time::minutes(1));

      if (nbRetry == 3)
         setPluginState(api, EWUPluginState::kNoConnection);

      ++nbRetry;

      throw CRequestErrorException();
   }
}