#include "stdafx.h"
#include "WeatherConditions.h"
#include "ErrorAnswerHandler.h"
#include <shared/exception/Exception.hpp>

CWeatherConditions::CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> api,
                                       IWUConfiguration& WUConfiguration,
                                       const std::string& PluginName,
                                       const std::string& Prefix)
   : m_Localisation(WUConfiguration.getLocalisation()),
     m_CountryOrState(WUConfiguration.getCountryOrState()),
     m_PluginName(PluginName),
     m_Temp(PluginName, Prefix + "temperature"),
     m_pressure(boost::make_shared<yApi::historization::CPressure>(Prefix + "pressure")),
     m_humidity(boost::make_shared<yApi::historization::CHumidity>(Prefix + "Humidity")),
     m_visibility(boost::make_shared<yApi::historization::CDistance>(Prefix + "Visibility")),
     m_uv(boost::make_shared<yApi::historization::CDirection>(Prefix + "UV")),
     m_WindDirection(boost::make_shared<yApi::historization::CDirection>(Prefix + "WindDirection")),
     m_DewPoint(PluginName, Prefix + "DewPoint"),
     m_Rain_1hr(PluginName, Prefix + "Rain_1hr"),
     m_WeatherConditionUrl(PluginName, Prefix + "WeatherCondition"),
     m_WindAverageSpeed(PluginName, Prefix + "windAverageSpeed"),
     m_WindMaxSpeed(PluginName, Prefix + "windMaxSpeed"),
     m_FeelsLike(PluginName, Prefix + "FeelsLike"),
     m_Windchill(PluginName, Prefix + "Windchill"),
     m_LiveConditions(PluginName, "LiveConditions")
{
   try
   {
      m_CatchError = false;

      m_URL.str("");
      m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/conditions/q/" << m_CountryOrState << "/" << m_Localisation << ".json";

      InitializeVariables(api, WUConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Weather condition module :" << e.what() << std::endl;

      // Informs Yadoms about the plugin actual state
      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");

      m_CatchError = true;
   }
}

void CWeatherConditions::InitializeVariables(boost::shared_ptr<yApi::IYPluginApi> api,
                                             IWUConfiguration& WUConfiguration) const
{
   shared::CDataContainer details;
   details.set("provider", "weather-underground");
   details.set("shortProvider", "wu");

   if (WUConfiguration.IsConditionsIndividualKeywordsEnabled())
   {
      if (!api->keywordExists(m_PluginName, m_pressure)) api->declareKeyword(m_PluginName, m_pressure, details);
      if (!api->keywordExists(m_PluginName, m_humidity)) api->declareKeyword(m_PluginName, m_humidity, details);
      if (!api->keywordExists(m_PluginName, m_visibility)) api->declareKeyword(m_PluginName, m_visibility, details);
      if (!api->keywordExists(m_PluginName, m_uv)) api->declareKeyword(m_PluginName, m_uv, details);
      if (!api->keywordExists(m_PluginName, m_WindDirection)) api->declareKeyword(m_PluginName, m_WindDirection, details);

      m_Temp.Initialize(api, details);
      m_DewPoint.Initialize(api, details);
      m_Rain_1hr.Initialize(api, details);
      m_WeatherConditionUrl.Initialize(api, details);
      m_WindAverageSpeed.Initialize(api, details);
      m_WindMaxSpeed.Initialize(api, details);
      m_FeelsLike.Initialize(api, details);
      m_Windchill.Initialize(api, details);
   }

   if (WUConfiguration.IsLiveConditionsEnabled())
   {
      m_LiveConditions.Initialize(api, details);

      m_LiveConditions.AddUnit(
         shared::plugin::yPluginApi::CStandardCapacities::Temperature.getName(),
         shared::plugin::yPluginApi::CStandardCapacities::Temperature.getUnit()
      );
      m_LiveConditions.AddUnit(
         shared::plugin::yPluginApi::CStandardCapacities::Speed.getName(),
         shared::plugin::yPluginApi::CStandardCapacities::Speed.getUnit()
      );
      m_LiveConditions.AddUnit(
         shared::plugin::yPluginApi::CStandardCapacities::Humidity.getName(),
         shared::plugin::yPluginApi::CStandardCapacities::Humidity.getUnit()
      );
      m_LiveConditions.AddUnit(
         shared::plugin::yPluginApi::CStandardCapacities::Rain.getName(),
         shared::plugin::yPluginApi::CStandardCapacities::Rain.getUnit()
      );
   }
}

void CWeatherConditions::OnUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& WUConfiguration)
{
   try
   {
      m_CatchError = false;

      InitializeVariables(api, WUConfiguration);

      //read the localisation
      m_Localisation = WUConfiguration.getLocalisation();

      //read the country or State code
      m_CountryOrState = WUConfiguration.getCountryOrState();

      m_URL.str("");
      m_URL << "http://api.wunderground.com/api/" << WUConfiguration.getAPIKey() << "/conditions/q/" << m_CountryOrState << "/" << m_Localisation << ".json";
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "Initialization Error");

      m_CatchError = true;
   }
}

bool CWeatherConditions::Request(boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (!m_CatchError)
   {
      try
      {
         m_data = m_webServer.SendGetRequest(m_URL.str());
      }
      catch (shared::exception::CException& e)
      {
         std::cout << "Weather Conditions :" << e.what() << std::endl;
         api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
         m_CatchError = true;
      }
   }

   if (!m_CatchError)
   {
      try
      {
         ErrorAnswerHandler Response(api, m_data);

         m_CatchError = Response.ContainError();
      }
      catch (...)
      {
      }
   }

   if (!m_CatchError)
   {
      try
      {
         m_CityConditions = m_data.get<std::string>("current_observation.observation_location.city");

         m_LiveConditions.SetCityName(m_CityConditions);

         std::cout << "Observation location :" << m_data.get<std::string>("current_observation.observation_location.full") << std::endl;
      }
      catch (shared::exception::CException& e)
      {
         std::cout << "Weather Conditions :" << e.what() << std::endl;
         m_CatchError = true;
      }
   }
   return m_CatchError;
}

std::string CWeatherConditions::GetCityName() const
{
   return m_CityConditions;
}

void CWeatherConditions::Parse(boost::shared_ptr<yApi::IYPluginApi> api,
                               const IWUConfiguration& WUConfiguration) const
{
   try
   {
      std::string error = m_data.getWithDefault<std::string>("response.error.description", "");

      if (!error.empty())
      {
         std::cerr << "ERROR : " << error << std::endl;
      }
      else
      {
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> KeywordList;

         if (WUConfiguration.IsConditionsIndividualKeywordsEnabled())
         {
            //
            //Temperature
            //
            m_Temp.SetValue(m_data, "current_observation.temp_c");
            KeywordList.push_back(m_Temp.GetHistorizable());

            //
            //Pressure
            //
            m_pressure->set(m_data.get<double>("current_observation.pressure_mb"));
            std::cout << m_pressure->getKeyword() << "=" << m_pressure->get() << "mbar" << std::endl;
            KeywordList.push_back(m_pressure);

            //
            //Humidity
            //

            std::string str_humidity = m_data.get<std::string>("current_observation.relative_humidity");
            str_humidity.erase(str_humidity.end() - 1);
            double d_humidity = static_cast<double>(atof(str_humidity.c_str()));
            m_humidity->set(d_humidity);
            std::cout << m_humidity->getKeyword() << "=" << m_humidity->get() << "%" << std::endl;
            KeywordList.push_back(m_humidity);

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
            KeywordList.push_back(m_visibility);

            //
            //UV
            //

            m_uv->set(static_cast<int>(m_data.get<double>("current_observation.UV")));
            std::cout << m_uv->getKeyword() << "=" << m_uv->get() << std::endl;
            KeywordList.push_back(m_uv);

            //
            //DewPoint
            //

            m_DewPoint.SetValue(m_data, "current_observation.dewpoint_c");
            KeywordList.push_back(m_DewPoint.GetHistorizable());

            //
            //Rain
            //

            m_Rain_1hr.SetValue(m_data, "current_observation.precip_today_metric");
            KeywordList.push_back(m_Rain_1hr.GetHistorizable());

            //
            //Visual condition
            //

            m_WeatherConditionUrl.SetValue(m_data, "current_observation.icon");
            KeywordList.push_back(m_WeatherConditionUrl.GetHistorizable());

            //
            //Wind (degrees)
            //

            m_WindDirection->set(static_cast<int>(m_data.get<double>("current_observation.wind_degrees")));
            std::cout << m_WindDirection->getKeyword() << "=" << m_WindDirection->get() << " degrees" << std::endl;
            KeywordList.push_back(m_WindDirection);

            //
            //Wind (speed)
            //

            m_WindAverageSpeed.SetValue(m_data, "current_observation.wind_kph");
            KeywordList.push_back(m_WindAverageSpeed.GetHistorizable());

            //
            //Wind (Max speed)
            //

            m_WindMaxSpeed.SetValue(m_data, "current_observation.wind_gust_kph");
            KeywordList.push_back(m_WindMaxSpeed.GetHistorizable());

            //
            //Feelslike
            //

            m_FeelsLike.SetValue(m_data, "current_observation.feelslike_c");
            KeywordList.push_back(m_FeelsLike.GetHistorizable());

            //
            //Windchill
            //

            m_Windchill.SetValue(m_data, "current_observation.windchill_c");
            KeywordList.push_back(m_Windchill.GetHistorizable());
         }

         if (WUConfiguration.IsLiveConditionsEnabled())
         {
            m_LiveConditions.SetPeriod(m_data,
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
            KeywordList.push_back(m_LiveConditions.GetHistorizable());
         }

         api->historizeData(m_PluginName, KeywordList);
      }
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl;
   }
}

bool CWeatherConditions::IsModuleInFault() const
{
   return m_CatchError;
}

CWeatherConditions::~CWeatherConditions()
{
}

