#include "stdafx.h"
#include "ForecastDays.h"
#include "ErrorAnswerHandler.h"
#include <shared/exception/Exception.hpp>

CForecastDays::CForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                             IWUConfiguration& wuConfiguration,
                             std::string deviceName,
                             const std::string& prefix)
   : m_localisation(wuConfiguration.getLocalisation()),
     m_countryOrState(wuConfiguration.getCountryOrState()),
     m_prefix(prefix),
     m_deviceName(deviceName),
     m_forecast(boost::make_shared<CForecast>(deviceName, "Forecast", weatherunderground::helper::EPeriod::kDay))
{
   try
   {
      m_catchError = false;
      m_url.str("");
      m_url << "http://api.wunderground.com/api/" << wuConfiguration.getAPIKey() << "/" << m_prefix << "/q/" << m_countryOrState << "/" << m_localisation << ".json";

      InitializeForecastDays(api, wuConfiguration);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Forecast 3 Days module :" << e.what() << std::endl;

      // Informs Yadoms about the plugin actual state
      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");

      m_catchError = true;
   }
}

void CForecastDays::InitializeForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                                           IWUConfiguration& wuConfiguration
)
{
   //Initialization
   try
   {
      // Clear the list
      m_keywords.clear();

      if (wuConfiguration.IsForecast10DaysEnabled())
      {
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
               m_forecastRain[counter] = boost::make_shared<CRain>(m_deviceName, TempString.str());
               m_keywords.push_back(m_forecastRain[counter]->getHistorizable());
            }
         }
      }

      // Declare keywords
      std::string m_URL = "www.wunderground.com/";
      api->declareDevice(m_deviceName, m_URL, m_keywords);
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Configuration or initialization error of Forecast 3 Days module :" << e.what() << std::endl;

      // Informs Yadoms about the plugin actual state
      api->setPluginState(yApi::historization::EPluginState::kCustom, "InitializationError");

      m_catchError = true;
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

bool CForecastDays::request(boost::shared_ptr<yApi::IYPluginApi> api)
{
   try
   {
      m_catchError = false;
      m_data = m_webServer.SendGetRequest(m_url.str());
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Forecast 10 days :" << e.what() << std::endl;
      api->setPluginState(yApi::historization::EPluginState::kCustom, "NoConnection");
      m_catchError = true;
   }

   return m_catchError;
}

void CForecastDays::parse(boost::shared_ptr<yApi::IYPluginApi> api,
                          const IWUConfiguration& wuConfiguration)
{
   try
   {
      ErrorAnswerHandler Response(api, m_data);
      m_catchError = Response.ContainError();

      if (!m_catchError)
      {
         if (wuConfiguration.IsForecast10DaysEnabled())
         {
            auto result = m_data.get<std::vector<shared::CDataContainer> >("forecast.simpleforecast.forecastday");
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

               if (wuConfiguration.IsRainIndividualKeywordsEnabled())
               {
                  if (counter < NB_RAIN_FORECAST_DAY)
                  {
                     m_forecastRain[counter]->setValue(*i, "qpf_allday.mm");
                  }
                  ++counter;
               }
            }
         }
         api->historizeData(m_deviceName, m_keywords);

         std::cout << "Forecast Updated !" << std::endl;
      }
   }
   catch (shared::exception::CException& e)
   {
      std::cout << "Error during the parsing of the element ! : " << e.what() << std::endl;
   }
}

void CForecastDays::setCityName(const std::string& CityName)
{
   m_forecast->setCityName(CityName);
}

bool CForecastDays::isModuleInFault() const
{
   return m_catchError;
}

CForecastDays::~CForecastDays()
{
}