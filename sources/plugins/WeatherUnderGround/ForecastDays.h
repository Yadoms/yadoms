#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "WeatherUndergroundHelpers.h"
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "WUConfiguration.h"
#include "Keywords/Forecast.h"
#include "Keywords/Rain.h"

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
   /// \param[in] api              pointer to the API
   /// \param[in] wuConfiguration  The Configuration of the module
   /// \param[in] pluginName       The Name of the module
   /// \param[in] prefix           Prefix Name used eventually to subname Keywords
   //--------------------------------------------------------------
   CForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                 IWUConfiguration& wuConfiguration,
                 std::string pluginName,
                 const std::string& prefix);

   //--------------------------------------------------------------
   /// \brief	  Send the request and receive the response from the web site
   /// \param[in] api            pointer to the API
   //--------------------------------------------------------------
   bool request(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] api             pointer to the API
   /// \param[in] wuConfiguration The configuration of the module
   /// \param[in] pluginName      The name of the plugin module
   //--------------------------------------------------------------
   void parse(boost::shared_ptr<yApi::IYPluginApi> api,
      const IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] api                pointer to the API
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void onUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                 IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Set the city Name
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void setCityName(const std::string& CityName);

   //--------------------------------------------------------------
   /// \brief	  Return true if an error occured during the request
   /// \return    The state of this request
   //--------------------------------------------------------------
   bool isModuleInFault() const;

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CForecastDays();

private:

   //--------------------------------------------------------------
   /// \brief	  Initialise Forecast 10 Days variables
   /// \param[in] api                pointer to the API
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void InitializeForecastDays(boost::shared_ptr<yApi::IYPluginApi> api,
                               IWUConfiguration& wuConfiguration);

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
   /// \brief	    The Plugin Name
   //--------------------------------------------------------------
   std::string m_pluginName;

   //--------------------------------------------------------------
   /// \brief	    Raw Web Data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    The url link to access properly the web site
   //--------------------------------------------------------------
   std::stringstream m_url;

   //--------------------------------------------------------------
   /// \brief	    The web Server engine
   //--------------------------------------------------------------
   shared::CHttpMethods m_webServer;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   CForecast m_forecast;

   //--------------------------------------------------------------
   /// \brief	    Tab of rain keywords for 3 next days
   //--------------------------------------------------------------
   boost::shared_ptr<CRain> m_forecastRain[NB_RAIN_FORECAST_DAY];

   //--------------------------------------------------------------
   /// \brief	    Error Detecting ?
   //--------------------------------------------------------------
   bool m_catchError;
};

