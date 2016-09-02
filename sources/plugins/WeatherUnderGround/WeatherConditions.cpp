#include "stdafx.h"
#include "WeatherConditions.h"
#include "ErrorAnswerHandler.h"
#include <shared/exception/Exception.hpp>

CWeatherConditions::CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> api,
                                       IWUConfiguration& wuConfiguration,
                                       const std::string& deviceName,
                                       const std::string& prefix)
   : m_localisation(wuConfiguration.getLocalisation()),
     m_countryOrState(wuConfiguration.getCountryOrState()),
     m_deviceName(deviceName),
     m_temp(boost::make_shared<CTemp>(deviceName, prefix + "temperature")),
     m_pressure(boost::make_shared<yApi::historization::CPressure>(prefix + "pressure")),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>(prefix + "Humidity")),
     m_visibility(boost::make_shared<yApi::historization::CDistance>(prefix + "Visibility")),
     m_uv(boost::make_shared<yApi::historization::CDirection>(prefix + "UV")),
     m_WindDirection(boost::make_shared<yApi::historization::CDirection>(prefix + "WindDirection")),
     m_dewPoint(boost::make_shared<CTemp>(deviceName, prefix + "DewPoint")),
     m_rain1hr(boost::make_shared<CRain>(deviceName, prefix + "Rain_1hr")),
     m_weatherConditionUrl(boost::make_shared<CWeatherIcon>(deviceName, prefix + "WeatherCondition")),
     m_windAverageSpeed(boost::make_shared<CWindSpeed>(deviceName, prefix + "windAverageSpeed")),
     m_windMaxSpeed(boost::make_shared<CWindSpeed>(deviceName, prefix + "windMaxSpeed")),
     m_feelsLike(boost::make_shared<CTemp>(deviceName, prefix + "FeelsLike")),
     m_windchill(boost::make_shared<CTemp>(deviceName, prefix + "Windchill")),
     m_liveConditions(boost::make_shared<CCondition>(deviceName, "LiveConditions"))
{
   try
   {
      m_catchError = false;

      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/conditions/q/" << m_countryOrState << "/" << m_localisation << ".json";

      initializeVariables(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Weather condition module :" << e.what() << std::endl;

      // Informs Yadoms about the plugin actual state
      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");

      m_catchError = true;
   }
}

void CWeatherConditions::initializeVariables(boost::shared_ptr<yApi::IYPluginApi> api,
                                             IWUConfiguration& wuConfiguration)
{
   // Clear the list
   m_keywords.clear();

   if (wuConfiguration.IsConditionsIndividualKeywordsEnabled())
   {
      m_keywords.push_back(m_temp->getHistorizable());
      m_keywords.push_back(m_pressure);
      m_keywords.push_back(m_humidity);
      m_keywords.push_back(m_visibility);
      m_keywords.push_back(m_uv);
      m_keywords.push_back(m_WindDirection);
      m_keywords.push_back(m_dewPoint->getHistorizable());
      m_keywords.push_back(m_rain1hr->getHistorizable());
      m_keywords.push_back(m_weatherConditionUrl->getHistorizable());
      m_keywords.push_back(m_windAverageSpeed->getHistorizable());
      m_keywords.push_back(m_windMaxSpeed->getHistorizable());
      m_keywords.push_back(m_feelsLike->getHistorizable());
      m_keywords.push_back(m_windchill->getHistorizable());
   }

   if (wuConfiguration.IsLiveConditionsEnabled())
   {
      m_keywords.push_back(m_liveConditions->getHistorizable());

      m_liveConditions->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Temperature.getName(),
                               shared::plugin::yPluginApi::CStandardCapacities::Temperature.getUnit());
      m_liveConditions->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Speed.getName(),
                               shared::plugin::yPluginApi::CStandardCapacities::Speed.getUnit());
      m_liveConditions->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Humidity.getName(),
                               shared::plugin::yPluginApi::CStandardCapacities::Humidity.getUnit());
      m_liveConditions->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Rain.getName(),
                               shared::plugin::yPluginApi::CStandardCapacities::Rain.getUnit());
   }

   // Declare keywords
   std::string m_URL = "www.wunderground.com/";
   api->declareDevice(m_deviceName, m_URL, m_keywords);
}

void CWeatherConditions::onUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration)
{
   try
   {
      m_catchError = false;

      initializeVariables(api, wuConfiguration);

      //read the localisation
      m_localisation = wuConfiguration.getLocalisation();

      //read the country or State code
      m_countryOrState = wuConfiguration.getCountryOrState();

      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/conditions/q/" << m_countryOrState << "/" << m_localisation << ".json";
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization Error");

      m_catchError = true;
   }
}

bool CWeatherConditions::request(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (!m_catchError)
   {
      try
      {
         m_data = m_webServer.SendGetRequest(m_url.str());
      }
      catch (shared::exception::CException& e)
      {
         std::cout << "Weather Conditions :" << e.what() << std::endl;
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
         m_catchError = true;
      }
   }

   if (!m_catchError)
   {
      try
      {
         ErrorAnswerHandler Response(api, m_data);

         m_catchError = Response.ContainError();
      }
      catch (...)
      {
      }
   }

   if (!m_catchError)
   {
      try
      {
         m_cityConditions = m_data.get<std::string>("current_observation.observation_location.city");

         m_liveConditions->setCityName(m_cityConditions);

         std::cout << "Observation location :" << m_data.get<std::string>("current_observation.observation_location.full") << std::endl;
      }
      catch (shared::exception::CException& e)
      {
         std::cout << "Weather Conditions :" << e.what() << std::endl;
         m_catchError = true;
      }
   }
   return m_catchError;
}

std::string CWeatherConditions::getCityName() const
{
   return m_cityConditions;
}

void CWeatherConditions::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                               const IWUConfiguration& wuConfiguration) const
{
   try
   {
      auto error = m_data.getWithDefault<std::string>("response.error.description", "");

      if (!error.empty())
      {
         std::cerr << "ERROR : " << error << std::endl;
      }
      else
      {
         if (wuConfiguration.IsConditionsIndividualKeywordsEnabled())
         {
            //
            //Temperature
            //
            m_temp->setValue(m_data, "current_observation.temp_c");

            //
            //Pressure
            //
            m_pressure->set(m_data.get<double>("current_observation.pressure_mb"));
            std::cout << m_pressure->getKeyword() << "=" << m_pressure->get() << "mbar" << std::endl;

            //
            //Humidity
            //
            auto str_humidity = m_data.get<std::string>("current_observation.relative_humidity");
            str_humidity.erase(str_humidity.end() - 1);
            auto d_humidity = static_cast<double>(atof(str_humidity.c_str()));
            m_humidity->set(d_humidity);
            std::cout << m_humidity->getKeyword() << "=" << m_humidity->get() << "%" << std::endl;

            //
            //Visibility
            //
            if (m_data.get<std::string>("current_observation.visibility_km") == "N/A")
               std::cout << m_visibility->getKeyword() << " : N/A => Value not registered" << std::endl;
            else
            {
               // x 1000 -> The visibility from the web site is in kilometer
               m_visibility->set(m_data.get<double>("current_observation.visibility_km") * 1000);
               std::cout << m_visibility->getKeyword() << "=" << m_visibility->get() << "m" << std::endl;
            }

            //
            //UV
            //
            m_uv->set(static_cast<int>(m_data.get<double>("current_observation.UV")));
            std::cout << m_uv->getKeyword() << "=" << m_uv->get() << std::endl;

            m_dewPoint->setValue(m_data, "current_observation.dewpoint_c");
            m_rain1hr->setValue(m_data, "current_observation.precip_today_metric");
            m_weatherConditionUrl->setValue(m_data, "current_observation.icon");
            m_WindDirection->set(static_cast<int>(m_data.get<double>("current_observation.wind_degrees")));
            std::cout << m_WindDirection->getKeyword() << "=" << m_WindDirection->get() << " degrees" << std::endl;
            m_windAverageSpeed->setValue(m_data, "current_observation.wind_kph");
            m_windMaxSpeed->setValue(m_data, "current_observation.wind_gust_kph");
            m_feelsLike->setValue(m_data, "current_observation.feelslike_c");
            m_windchill->setValue(m_data, "current_observation.windchill_c");
         }

         if (wuConfiguration.IsLiveConditionsEnabled())
         {
            m_liveConditions->setPeriod(m_data,
                                       "current_observation.local_time_rfc822",
                                       "current_observation.icon",
                                       "current_observation.temp_c",
                                       "current_observation.pressure_mb",
                                       "current_observation.visibility_km",
                                       "current_observation.UV",
                                       "current_observation.dewpoint_c",
                                       "current_observation.wind_gust_kph",
                                       "current_observation.wind_kph",
                                       "current_observation.wind_degrees",
                                       "current_observation.relative_humidity",
                                       "current_observation.precip_today_metric",
                                       "current_observation.feelslike_c",
                                       "current_observation.windchill_c"
            );
         }

         api->historizeData(m_deviceName, m_keywords);
      }
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
   }
}

bool CWeatherConditions::isModuleInFault() const
{
   return m_catchError;
}

CWeatherConditions::~CWeatherConditions()
{
}