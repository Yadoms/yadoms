#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../WeatherUndergroundHelpers.h"
#include <shared/DataContainer.h>
#include "IFeature.h"
#include "../IWUConfiguration.h"
#include "Keywords/Forecast.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Forecast X Days Module
/// \note   This class consist of the traitment of the forecast information during X days from the web site
//--------------------------------------------------------------
class CForecastDays : public features::IFeature
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] api                  pointer to the API
   /// \param[in] wuConfiguration      The Configuration of the module
   /// \param[in] deviceConfiguration  The Configuration of the device
   /// \param[in] location             GPS Position where to find forecast
   /// \param[in] deviceName           Name of the device
   /// \param[in] stationName          Name of the forecast station
   //--------------------------------------------------------------
   CForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                 IWUConfiguration& wuConfiguration,
                 boost::shared_ptr<const shared::ILocation> location,
                 const std::string& deviceName,
                 const std::string& stationName);

   // IFeature Implementation
   void parse(boost::shared_ptr<yApi::IYPluginApi> api, const shared::CDataContainer dataToParse, IWUConfiguration& wuConfiguration) override;
   void onPluginUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration) override;
//   void onDeviceUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration, boost::shared_ptr<const shared::ILocation> location) override;
   std::string getUrl() const override;
   std::string getName() const override;
   void setCityName(const std::string& CityName) override;
   // [END] IFeature Implementation

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CForecastDays();

private:

   //--------------------------------------------------------------
   /// \brief	  Initialise Forecast 10 Days variables
   /// \param[in] api                    pointer to the API
   //--------------------------------------------------------------
   void InitializeForecastDays(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_localisation;

   //--------------------------------------------------------------
   /// \brief	    url code between 3 or 10 days
   //--------------------------------------------------------------
   std::string m_prefix;

   //--------------------------------------------------------------
   /// \brief	    The Device Name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	    The url link to access properly the web site
   //--------------------------------------------------------------
   std::stringstream m_url;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   boost::shared_ptr<CForecast> m_forecast;

   //--------------------------------------------------------------
   /// \brief	    Tab of rain keywords for 3 next days
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CRainRate> m_forecastRain[NB_RAIN_FORECAST_DAY];

   //--------------------------------------------------------------
   /// \brief	    the temperature for the next day
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temp;

   //--------------------------------------------------------------
   /// \brief  Keywords list
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;

   //--------------------------------------------------------------
   /// \brief  developerMode state
   //--------------------------------------------------------------
   bool m_isDeveloperMode;

   //--------------------------------------------------------------
   /// \brief  the location used
   //--------------------------------------------------------------
   boost::shared_ptr<const shared::ILocation> m_location;
};