#include "stdafx.h"
#include "ForecastDays.h"
#include <shared/exception/Exception.hpp>
#include "Keywords/KeywordHelpers.h"
#include <shared/Log.h>

CForecastDays::CForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                             IWUConfiguration& wuConfiguration,
                             boost::shared_ptr<const shared::ILocation> location,
                             const std::string& deviceName,
                             const std::string& stationName)
   : m_localisation(stationName),
     m_deviceName(deviceName),
     m_forecast(boost::make_shared<CForecast>(m_deviceName, "Forecast", weatherunderground::helper::EPeriod::kDay)),
     m_url ("http://api.wunderground.com/api/" + wuConfiguration.getAPIKey() + "/forecast/q/" + boost::lexical_cast<std::string>(location->latitude()) + "," + boost::lexical_cast<std::string>(location->longitude()) + ".json"),
     m_isDeveloperMode(false),
     m_location(location)
{
   try
   {
      InitializeForecastDays(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Configuration or initialization error of the forecast module :" << e.what() ;
      throw;
   }
}

void CForecastDays::InitializeForecastDays(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration)
{
   if (wuConfiguration.isForecast10DaysEnabled())
   {
      m_keywords.clear();

      m_keywords.push_back(m_forecast->getHistorizable());

      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Temperature().getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Temperature().getUnit());
      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Speed().getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Speed().getUnit());
      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Humidity().getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Humidity().getUnit());
      m_forecast->addUnit(shared::plugin::yPluginApi::CStandardCapacities::Rain().getName(),
                          shared::plugin::yPluginApi::CStandardCapacities::Rain().getUnit());

      if (wuConfiguration.isRainIndividualKeywordsEnabled())
      {
         for (auto counter = 0; counter < NB_INDIVIDUAL_FORECAST_KEYWORDS; ++counter)
         {
            std::string TempString;
            TempString = "Rain_Day_" + boost::lexical_cast<std::string>(counter);
            m_forecastRain[counter] = boost::make_shared<yApi::historization::CRainRate>(TempString);
            m_keywords.push_back(m_forecastRain[counter]);
         }
      }

      if (wuConfiguration.isTempIndividualKeywordsEnabled())
      {
         for (auto counter = 0; counter < NB_INDIVIDUAL_FORECAST_KEYWORDS; ++counter)
         {
            std::string TempString;
            TempString = "lowtemp_Day_" + boost::lexical_cast<std::string>(counter);
            m_lowtemp[counter] = boost::make_shared<yApi::historization::CTemperature>(TempString);
            m_keywords.push_back(m_lowtemp[counter]);
            TempString = "hightemp_Day_" + boost::lexical_cast<std::string>(counter);
            m_hightemp[counter] = boost::make_shared<yApi::historization::CTemperature>(TempString);
            m_keywords.push_back(m_hightemp[counter]);
         }
      }

      // in a separate try/catch to work with old configurations
      try {
         if (wuConfiguration.isConditionsForecastIndividualKeywordsEnabled())
         {
            for (auto counter = 0; counter < NB_INDIVIDUAL_FORECAST_KEYWORDS; ++counter)
            {
               std::string TempString;
               TempString = "WeatherConditions_" + boost::lexical_cast<std::string>(counter);
               m_weatherCondition[counter] = boost::make_shared<yApi::historization::CWeatherCondition>(TempString);
            }
         }
      }
      catch (shared::exception::CException& e)
      {
         YADOMS_LOG(trace) << "This keyword doesn't exist yet ! : " << e.what();
      }

      // Declare keywords
      std::string m_type = "forecast";
      shared::CDataContainer details;
      details.set("type", m_type);
      api->declareDevice(m_deviceName, m_type, m_type, m_keywords, details);
   }
}

void CForecastDays::cleanUpKeywords(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration)
{
   if (!wuConfiguration.isRainIndividualKeywordsEnabled())
   {
      if (api->deviceExists(m_deviceName))
      {
         // reset all forecast rain keywords
         for (auto counter = 0; counter < NB_INDIVIDUAL_FORECAST_KEYWORDS; ++counter)
         {
            if (m_forecastRain[counter])
            {
               if (api->keywordExists(m_deviceName, m_forecastRain[counter]))
               {
                  api->removeKeyword(m_deviceName, m_forecastRain[counter]->getKeyword());
                  m_forecastRain[counter].reset();
               }
            }
         }
      }
   }

   if (!wuConfiguration.isTempIndividualKeywordsEnabled())
   {
      if (api->deviceExists(m_deviceName))
      {
         // reset all temp keywords
         for (auto counter = 0; counter < NB_INDIVIDUAL_FORECAST_KEYWORDS; ++counter)
         {
            if (m_lowtemp[counter])
            {
               if (api->keywordExists(m_deviceName, m_lowtemp[counter]))
               {
                  api->removeKeyword(m_deviceName, m_lowtemp[counter]->getKeyword());
                  m_lowtemp[counter].reset();
               }
            }
            if (m_hightemp[counter])
            {
               if (api->keywordExists(m_deviceName, m_hightemp[counter]))
               {
                  api->removeKeyword(m_deviceName, m_hightemp[counter]->getKeyword());
                  m_hightemp[counter].reset();
               }
            }
         }
      }
   }

   // In a separate try/catch to work with old configurations
   try {
      if (!wuConfiguration.isConditionsForecastIndividualKeywordsEnabled())
      {
         if (api->deviceExists(m_deviceName))
         {
            // reset all weatherCondition keywords
            for (auto counter = 0; counter < NB_INDIVIDUAL_FORECAST_KEYWORDS; ++counter)
            {
               if (m_weatherCondition[counter])
               {
                  if (api->keywordExists(m_deviceName, m_weatherCondition[counter]))
                  {
                     api->removeKeyword(m_deviceName, m_weatherCondition[counter]->getKeyword());
                     m_weatherCondition[counter].reset();
                  }
               }
            }
         }
      }
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(trace) << "This keyword doesn't exist yet ! : " << e.what();
   }
}

void CForecastDays::onPluginUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                   IWUConfiguration& wuConfiguration)
{
   try
   {
      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/forecast/q/" << boost::lexical_cast<std::string>(m_location->latitude()) << "," << boost::lexical_cast<std::string>(m_location->longitude()) << ".json";

      InitializeForecastDays(api, wuConfiguration);
      cleanUpKeywords(api, wuConfiguration);
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Configuration or initialization error in the forecast module :" << e.what() ;
      throw;
   }
}

void CForecastDays::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                          const shared::CDataContainer dataToParse,
                          IWUConfiguration& wuConfiguration)
{
   try
   {
      dataToParse.printToLog(YADOMS_LOG(trace));

      if (wuConfiguration.isForecast10DaysEnabled())
      {
         auto result = dataToParse.get<std::vector<shared::CDataContainer> >("forecast.simpleforecast.forecastday");
         std::vector<shared::CDataContainer>::iterator i;

         m_forecast->clearAllPeriods();
         m_forecast->setCityName(m_localisation);
         unsigned char counter = 0;

         dataToParse.printToLog(YADOMS_LOG(trace));

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

            if (wuConfiguration.isRainIndividualKeywordsEnabled())
            {
               if (counter < NB_INDIVIDUAL_FORECAST_KEYWORDS)
               {
                  double rainRate1h = 0;
                  if (convertDouble(rainRate1h, *i, "qpf_allday.mm"))
                     m_forecastRain[counter]->set(rainRate1h);
               }
            }

            if (wuConfiguration.isTempIndividualKeywordsEnabled())
            {
               if (counter == 0)
               {
                  double temp = 0;
                  if (convertDouble(temp, *i, "low.celsius"))
                     m_lowtemp[counter]->set(temp);

                  if (convertDouble(temp, *i, "high.celsius"))
                     m_hightemp[counter]->set(temp);
               }
            }
            try {
               if (wuConfiguration.isConditionsForecastIndividualKeywordsEnabled())
               {
                  if (counter == 0)
                  {
                     m_weatherCondition[counter]->set((*i).get<yApi::historization::EWeatherCondition>("icon"));
                  }
               }
            }
            catch (shared::exception::CException& e)
            {
               YADOMS_LOG(trace) << "This keyword doesn't exist yet ! : " << e.what();
            }

            ++counter;
         }
      }
      api->historizeData(m_deviceName, m_keywords);

      YADOMS_LOG(information) << "Refresh forecast information" ;
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Error during parsing the element ! : " << e.what() ;
   }
}

void CForecastDays::setCityName(const std::string& CityName)
{
   m_localisation = CityName;
}

CForecastDays::~CForecastDays()
{
}

std::string CForecastDays::getUrl() const
{
   return m_url.str();
}

std::string CForecastDays::getName() const
{
   return m_deviceName;
}