#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "WUConfiguration.h"
#include "Keywords/Barometric.h"
#include "Keywords/Humidity.h"
#include "Keywords/Rain.h"
#include "Keywords/Temp.h"
#include "Keywords/UV.h"
#include "Keywords/Visibility.h"
#include "Keywords/WeatherIcon.h"
#include "Keywords/WindDirection.h"
#include "Keywords/WindSpeed.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	WeatherConditions Module
/// \note   This class consist of the traitment of the weather conditions information from the web site
//--------------------------------------------------------------
class CWeatherConditions
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] context          pointer to the API
   /// \param[in] WUConfiguration  The Configuration of the module
   //--------------------------------------------------------------
   CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix);

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
   void Parse( boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration );

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] WUConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void OnUpdate( const IWUConfiguration& WUConfiguration );

   //--------------------------------------------------------------
   /// \brief	  Return the conditions city name
   //--------------------------------------------------------------
   std::string GetCityName ();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWeatherConditions();

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
   /// \brief	    The name of the city (country ? state ?)
   //--------------------------------------------------------------
   std::string m_CityConditions;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   CTemp        m_Temp;
   CBarometric  m_Pressure;
   CHumidity    m_Humidity;
   CVisibility  m_Visibility;
   CUV          m_UV;
   CTemp        m_DewPoint;
   CRain        m_Rain_1hr;
   CWeatherIcon m_WeatherConditionUrl;
   CWindDirection  m_WindDirection;
   CWindSpeed  m_WindAverageSpeed;
   CWindSpeed  m_WindMaxSpeed;
   CTemp        m_FeelsLike;
   CTemp        m_Windchill;
};

