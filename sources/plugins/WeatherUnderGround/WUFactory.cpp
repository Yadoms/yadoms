#include "stdafx.h"
#include "WUFactory.h"
#include "Features/WeatherConditions.h"
#include "Features/Astronomy.h"
#include "Features/ForecastDays.h"
#include "deviceConfiguration.h"
#include "WeatherUnderground.h"

CWUFactory::CWUFactory(boost::shared_ptr<yApi::IYPluginApi> api, 
                       IWUConfiguration& wuConfiguration)
{
   std::vector<std::string> devices = api->getAllDevices();
   std::vector<std::string>::iterator devicesIterator;

   // Create all devices if present
   for (devicesIterator = devices.begin(); devicesIterator != devices.end(); ++devicesIterator)
   {
	   std::string type = "";
      // plugin state have no type
	   try {
		   type = api->getDeviceDetails(*devicesIterator).get<std::string>("type");
	   }
	   catch (...)
	   {}

      std::cout << "Name : " << (*devicesIterator) << std::endl;
      std::cout << "type : " << type << std::endl;

      if (type == "WeatherConditions")
      {
         m_weatherConditions = boost::make_shared<CWeatherConditions>(api, 
                                                                      wuConfiguration, 
                                                                      boost::make_shared<CdeviceConfiguration>(api->getDeviceConfiguration(*devicesIterator)),
                                                                      (*devicesIterator));
      }
      else if (type == "Astronomy")
      {
         m_astronomy = boost::make_shared<CAstronomy>(api, 
                                                      wuConfiguration, 
                                                      boost::make_shared<CdeviceConfiguration>(api->getDeviceConfiguration(*devicesIterator)),
                                                      (*devicesIterator));
      }
      else if (type == "Forecast")
      {
         m_forecast = boost::make_shared<CForecastDays>(api, 
                                                    wuConfiguration, 
                                                    boost::make_shared<CdeviceConfiguration>(api->getDeviceConfiguration(*devicesIterator)),
                                                    (*devicesIterator));
      }  
   }
}

std::string CWUFactory::createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                             IWUConfiguration& wuConfiguration,
                                             boost::shared_ptr<yApi::IManuallyDeviceCreationRequest> request,
                                             boost::shared_ptr<const shared::ILocation> location)
{
   boost::shared_ptr<features::IFeature> returnModule;

   boost::shared_ptr<IdeviceConfiguration> deviceConfiguration = boost::make_shared<CdeviceConfiguration>(request->getData().getConfiguration());

   // If astronomy is enabled
   if (deviceConfiguration->isAstronomyEnabled())
   {
      if (!m_astronomy)
      {
         m_astronomy = boost::make_shared<CAstronomy>(api, wuConfiguration, deviceConfiguration, request->getData().getDeviceName());
         request->sendSuccess(m_astronomy->getName());
         returnModule = m_astronomy;

         api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

         // TODO : Voir pour ne pas créer plusieurs fois ce timer
         api->getEventHandler().createTimer(kEvtTimerRefreshAstronomy, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(9));
      }
      else
      {
         std::cout << "Could not create device " << request->getData().getDeviceName() << " : already exist." << std::endl;
         request->sendError("device already exist !"); // TODO : Traduire en anglais
      }
   }

   // if conditions are enabled
   if (deviceConfiguration->isConditionsIndividualKeywordsEnabled() || deviceConfiguration->isLiveConditionsEnabled())
   {
      if (!m_weatherConditions)
      {
         m_weatherConditions = boost::make_shared<CWeatherConditions>(api, wuConfiguration, deviceConfiguration, request->getData().getDeviceName());
         request->sendSuccess(m_weatherConditions->getName());
         returnModule = m_astronomy;

         api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

         // TODO : Voir pour ne pas créer plusieurs fois ce timer
         api->getEventHandler().createTimer(kEvtTimerRefreshWeatherConditions, shared::event::CEventTimer::kPeriodic, boost::posix_time::minutes(15));
      }
      else
      {
         std::cout << "Could not create device " << request->getData().getDeviceName() << " : already exist." << std::endl;
         request->sendError("device already exist !"); // TODO : Traduire en anglais
      }
   }

   // If forecast is enabled
   if (deviceConfiguration->isForecast10DaysEnabled())
   {
      if (!m_forecast)
      {
         m_forecast = boost::make_shared<CForecastDays>(api, wuConfiguration, deviceConfiguration, request->getData().getDeviceName());
         request->sendSuccess(m_forecast->getName()); 
         returnModule = m_astronomy;

         api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

         // TODO : Voir pour ne pas créer plusieurs fois ce timer
         api->getEventHandler().createTimer(kEvtTimerRefreshForecast10Days, shared::event::CEventTimer::kPeriodic, boost::posix_time::hours(3));
      }
      else
      {
         std::cout << "Could not create device " << request->getData().getDeviceName() << " : already exist." << std::endl;
         request->sendError("device already exist !"); // TODO : Traduire en anglais
      }
   }

   return returnModule->getName();
}

void CWUFactory::removeDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceRemoved)
{
   if (m_weatherConditions->getName() == deviceRemoved)
   {
      m_weatherConditions.reset();
   }
   else if (m_astronomy->getName() == deviceRemoved)
   {
      m_astronomy.reset();
   }
   else if (m_forecast->getName() == deviceRemoved)
   {
      m_forecast.reset();
   }
   else
   {
      std::cerr << "device " << deviceRemoved << " not found" << std::endl;
   }
}

boost::shared_ptr<features::IFeature> CWUFactory::getWeatherConditionsDevice()
{
   return m_weatherConditions;
}

boost::shared_ptr<features::IFeature> CWUFactory::getAstronomyDevice()
{
   return m_astronomy;
}

boost::shared_ptr<features::IFeature> CWUFactory::getForecastDevice()
{
   return m_forecast;
}

CWUFactory::~CWUFactory()
{}