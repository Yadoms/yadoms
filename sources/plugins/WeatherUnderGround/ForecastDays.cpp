#include "stdafx.h"
#include "ForecastDays.h"
#include <shared/exception/Exception.hpp>
#include "Keywords/KeywordHelpers.h"

CForecastDays::CForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                             IWUConfiguration& wuConfiguration,
                             const std::string& deviceName,
                             const std::string& prefix)
   : m_localisation(wuConfiguration.getLocalisation()),
     m_countryOrState(wuConfiguration.getCountryOrState()),
     m_prefix(prefix),
     m_deviceName(deviceName),
     m_forecast(boost::make_shared<CForecast>(deviceName, "Forecast", weatherunderground::helper::EPeriod::kDay)),
     m_temp(boost::make_shared<yApi::historization::CTemperature>(prefix + "low_temperature")),
     m_isDeveloperMode(false)
{
   try
   {
      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/forecast/q/" << m_countryOrState << "/" << m_localisation << ".json";

      InitializeForecastDays(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Forecast 3 Days module :" << e.what() << std::endl;
      throw;
   }
}

void CForecastDays::InitializeForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                                           IWUConfiguration& wuConfiguration
)
{
   if (wuConfiguration.IsForecast10DaysEnabled())
   {
      m_keywords.clear();

      m_keywords.push_back(m_forecast->getHistorizable());

      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Temperature.getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Temperature.getUnit());
      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Speed.getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Speed.getUnit());
      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Humidity.getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Humidity.getUnit());
      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Rain.getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Rain.getUnit());

      if (wuConfiguration.IsRainIndividualKeywordsEnabled())
      {
         for (auto counter = 0; counter < NB_RAIN_FORECAST_DAY; ++counter)
         {
            std::stringstream TempString;
            TempString << m_prefix << "Rain_Day_" << counter;
            m_forecastRain[counter] = boost::make_shared<yApi::historization::CRainRate>(TempString.str());
            m_keywords.push_back(m_forecastRain[counter]);
         }
      }

      m_keywords.push_back(m_temp);

      // Declare keywords
      std::string m_URL = "www.wunderground.com/";
      api->declareDevice(m_deviceName, m_URL, m_keywords);
   }
}

void CForecastDays::onUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                             IWUConfiguration& wuConfiguration)
{
   //read the localisation
   m_localisation = wuConfiguration.getLocalisation();

   //read the country or State code
   m_countryOrState = wuConfiguration.getCountryOrState();

   m_url.str("");

   m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/" << m_prefix << "/q/" << m_countryOrState << "/" << m_localisation << ".json";

   InitializeForecastDays(api, wuConfiguration);
}

void CForecastDays::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                          const IWUConfiguration& wuConfiguration,
                          const shared::CDataContainer dataToParse) const
{
   try
   {
      if (wuConfiguration.IsForecast10DaysEnabled())
      {
         auto result = dataToParse.get<std::vector<shared::CDataContainer> >("forecast.simpleforecast.forecastday");
         std::vector<shared::CDataContainer>::iterator i;

         m_forecast->clearAllPeriods();

         unsigned char counter = 0;

         for (i = result.begin(); i != result.end(); ++i)
         {
            m_forecast->addPeriod(*i,
                                    "date.year",
                                    "date.month",
                                    "date.day",
                                    "icon",
                                    "high.celsius",
                                    "low.celsius",
                                    "maxwind.kph",
                                    "avewind.kph",
                                    "avewind.degrees",
                                    "avehumidity",
                                    "qpf_allday.mm",
                                    "snow_allday.cm");

            if (counter == 0)
            {
               double temp = 0;
               if (convertDouble(temp, *i, "low.celsius"))
                  m_temp->set(temp);
            }

            if (wuConfiguration.IsRainIndividualKeywordsEnabled())
            {
               if (counter < NB_RAIN_FORECAST_DAY)
               {
                  double rainRate1h = 0;
                  if (convertDouble(rainRate1h, *i, "qpf_allday.mm"))
                     m_forecastRain[counter]->set(rainRate1h);
               }
            }

            ++counter;
         }
      }
      api->historizeData(m_deviceName, m_keywords);

      std::cout << "Refresh Forecast Information" << std::endl;
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Error during parsing the element ! : " << e.what() << std::endl;
   }
}

void CForecastDays::setCityName(const std::string& CityName) const
{
   m_forecast->setCityName(CityName);
}

CForecastDays::~CForecastDays()
{
}

std::string CForecastDays::getUrl() const
{
   return m_url.str();
}