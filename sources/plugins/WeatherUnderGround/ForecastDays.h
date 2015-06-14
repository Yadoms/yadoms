#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "WeatherUndergroundHelpers.h"
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "WUConfiguration.h"
#include "Keywords/WeatherIcon.h"
#include "Keywords/Temp.h"
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
   /// \param[in] context          pointer to the API
   /// \param[in] WUConfiguration  The Configuration of the module
   /// \param[in] PluginName       The Name of the module
   /// \param[in] Prefix           Prefix Name used eventually to subname Keywords
   //--------------------------------------------------------------
   CForecastDays(boost::shared_ptr<yApi::IYPluginApi> context, 
                  IWUConfiguration& WUConfiguration, 
                  std::string PluginName, 
                  const std::string & Prefix);

   //--------------------------------------------------------------
   /// \brief	  Send the request and receive the response from the web site
   /// \param[in] context    pointer to the API
   //--------------------------------------------------------------
   void Request( boost::shared_ptr<yApi::IYPluginApi> context );

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] context         pointer to the API
   /// \param[in] WUConfiguration The configuration of the module
   /// \param[in] PluginName      The name of the plugin module
   //--------------------------------------------------------------
   void Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] WUConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void OnUpdate( IWUConfiguration& WUConfiguration );

   //--------------------------------------------------------------
   /// \brief	  Set the city Name
   /// \param[in] WUConfiguration    The Plugin configuration
   //--------------------------------------------------------------

   void SetCityName ( const std::string & CityName );

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CForecastDays();

private:

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_Localisation;

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_CountryOrState;

   //--------------------------------------------------------------
   /// \brief	    url code between 3 or 10 days
   //--------------------------------------------------------------
   std::string m_Prefix;

   //--------------------------------------------------------------
   /// \brief	    The Plugin Name
   //--------------------------------------------------------------
   std::string m_PluginName;

   //--------------------------------------------------------------
   /// \brief	    Raw Web Data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    The url link to access properly the web site
   //--------------------------------------------------------------
   std::stringstream m_URL;

   //--------------------------------------------------------------
   /// \brief	    The web Server engine
   //--------------------------------------------------------------
   shared::CHttpMethods m_webServer;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   CForecast    m_Forecast;

   //--------------------------------------------------------------
   /// \brief	    Tab of rain keywords for 3 next days
   //--------------------------------------------------------------
   boost::shared_ptr<CRain>        m_Forecast_Rain[NB_RAIN_FORECAST_DAY];
};

