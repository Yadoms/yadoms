#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "WeatherUndergroundHelpers.h"
#include <shared/DataContainer.h>
#include "IWUConfiguration.h"
#include "IdeviceConfiguration.h"
#include "Keywords/Forecast.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Forecast X Days Module
/// \note   This class consist of the traitment of the forecast information during X days from the web site
//--------------------------------------------------------------
class CForecastDays
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] api                  pointer to the API
   /// \param[in] wuConfiguration      The Configuration of the module
   /// \param[in] deviceConfiguration  The Configuration of the device
   /// \param[in] deviceName           Name of the device
   //--------------------------------------------------------------
   CForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                 IWUConfiguration& wuConfiguration,
                 boost::shared_ptr<IdeviceConfiguration> deviceConfiguration,
                 const std::string& deviceName);

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] api             pointer to the API
   /// \param[in] dataToParse     data to parse
   //--------------------------------------------------------------
   void parse(boost::shared_ptr<yApi::IYPluginApi> api,
              const shared::CDataContainer dataToParse) const;

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] api                    pointer to the API
   /// \param[in] wuConfiguration        The Plugin configuration
   //--------------------------------------------------------------
   void onPluginUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                       IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] api                    pointer to the API
   /// \param[in] deviceConfiguration    The configuration of the device
   //--------------------------------------------------------------
   void onDeviceUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                       boost::shared_ptr<IdeviceConfiguration> deviceConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Set the city Name
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void setCityName(const std::string& CityName) const;

   //--------------------------------------------------------------
   /// \brief	  Return the url
   /// \return    The url string
   //--------------------------------------------------------------
   std::string getUrl() const;

   //--------------------------------------------------------------
   /// \brief	  Return the name of the device
   /// \return    The url string
   //--------------------------------------------------------------
   std::string getName() const;

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CForecastDays();

private:

   //--------------------------------------------------------------
   /// \brief	  Initialise Forecast 10 Days variables
   /// \param[in] api                    pointer to the API
   //--------------------------------------------------------------
   void InitializeForecastDays(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_localisation;

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_countryOrState;

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
   /// \brief	    The device Configuration
   //--------------------------------------------------------------   
   boost::shared_ptr<IdeviceConfiguration> m_deviceConfiguration;
};