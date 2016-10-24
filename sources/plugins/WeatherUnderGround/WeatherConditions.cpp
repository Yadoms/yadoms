#include "stdafx.h"
#include "WeatherConditions.h"
#include <shared/exception/Exception.hpp>
#include "Keywords/KeywordException.hpp"

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
     m_liveConditions(boost::make_shared<CCondition>(deviceName, "LiveConditions")),
     m_isDesactivated(false),
     m_isUserDesactivated(false)
{
   try
   {
      initializeVariables(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Weather condition module :" << e.what() << std::endl;
      m_isDesactivated = true;
      throw e;
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

   if (wuConfiguration.IsConditionsIndividualKeywordsEnabled() || wuConfiguration.IsLiveConditionsEnabled())
   {
      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/conditions/q/" << m_countryOrState << "/" << m_localisation << ".json";

      // Declare keywords
      std::string m_URL = "www.wunderground.com/";
      api->declareDevice(m_deviceName, m_URL, m_keywords);

      m_isUserDesactivated = false;
   }
   else
      m_isUserDesactivated = true;
}

void CWeatherConditions::onUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration)
{
   try
   {
      //read the localisation
      m_localisation = wuConfiguration.getLocalisation();

      //read the country or State code
      m_countryOrState = wuConfiguration.getCountryOrState();

      initializeVariables(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
      m_isDesactivated = true;
      throw e;
   }
}

std::string CWeatherConditions::getUrl() const
{
   return m_url.str();
}

std::string CWeatherConditions::getCityName() const
{
   return m_cityConditions;
}

void CWeatherConditions::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                               const IWUConfiguration& wuConfiguration,
                               const shared::CDataContainer dataToParse)
{
   if (!m_isDesactivated && !m_isUserDesactivated)
   {
      try
      {
         m_cityConditions = dataToParse.get<std::string>("current_observation.observation_location.city");
         m_liveConditions->setCityName(m_cityConditions);
         std::cout << "Observation location :" << dataToParse.get<std::string>("current_observation.observation_location.full") << std::endl;

         if (wuConfiguration.IsConditionsIndividualKeywordsEnabled())
         {
            //
            //Temperature
            //
            m_temp->setValue(dataToParse, "current_observation.temp_c");

            //
            //Pressure
            //
            m_pressure->set(dataToParse.get<double>("current_observation.pressure_mb"));
            std::cout << m_pressure->getKeyword() << "=" << m_pressure->get() << "mbar" << std::endl;

            //
            //Humidity
            //
            auto str_humidity = dataToParse.get<std::string>("current_observation.relative_humidity");
            str_humidity.erase(str_humidity.end() - 1);
            auto d_humidity = static_cast<double>(atof(str_humidity.c_str()));
            m_humidity->set(d_humidity);
            std::cout << m_humidity->getKeyword() << "=" << m_humidity->get() << "%" << std::endl;

            //
            //Visibility
            //
            if (dataToParse.get<std::string>("current_observation.visibility_km") == "N/A")
               std::cout << m_visibility->getKeyword() << " : N/A => Value not registered" << std::endl;
            else
            {
               // x 1000 -> The visibility from the web site is in kilometer
               m_visibility->set(dataToParse.get<double>("current_observation.visibility_km") * 1000);
               std::cout << m_visibility->getKeyword() << "=" << m_visibility->get() << "m" << std::endl;
            }

            //
            //UV
            //
            m_uv->set(static_cast<int>(dataToParse.get<double>("current_observation.UV")));
            std::cout << m_uv->getKeyword() << "=" << m_uv->get() << std::endl;

            m_dewPoint->setValue(dataToParse, "current_observation.dewpoint_c");
            m_rain1hr->setValue(dataToParse, "current_observation.precip_today_metric");
            m_weatherConditionUrl->setValue(dataToParse, "current_observation.icon");
            m_WindDirection->set(static_cast<int>(dataToParse.get<double>("current_observation.wind_degrees")));
            std::cout << m_WindDirection->getKeyword() << "=" << m_WindDirection->get() << " degrees" << std::endl;
            m_windAverageSpeed->setValue(dataToParse, "current_observation.wind_kph");
            m_windMaxSpeed->setValue(dataToParse, "current_observation.wind_gust_kph");
            m_feelsLike->setValue(dataToParse, "current_observation.feelslike_c");
            m_windchill->setValue(dataToParse, "current_observation.windchill_c");
         }

         if (wuConfiguration.IsLiveConditionsEnabled())
         {
            m_liveConditions->setPeriod(dataToParse,
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

         std::cout << "Refresh Weather Conditions" << std::endl;
      }
      catch (CKeywordException&)
      {}
      catch (shared::exception::CException& e)
      {
         std::cout << e.what() << std::endl;
         m_isDesactivated = true;
      }
   }
}

CWeatherConditions::~CWeatherConditions()
{}

bool CWeatherConditions::isDesactivated() const
{
   return m_isDesactivated;
}

bool CWeatherConditions::isUserDesactivated() const
{
   return m_isUserDesactivated;
}