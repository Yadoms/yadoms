#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "WUConfiguration.h"
#include "Keywords/Temp.h"
#include "Keywords/WeatherIcon.h"
#include "Keywords/WindSpeed.h"
#include "Keywords/Condition.h"
#include "Keywords/Rain.h"

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
   /// \param[in] api              pointer to the API
   /// \param[in] WUConfiguration  The Configuration of the module
   //--------------------------------------------------------------
   CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& WUConfiguration, const std::string& PluginName, const std::string& Prefix);

   //--------------------------------------------------------------
   /// \brief	  Send the request and receive the response from the web site
   /// \param[in] api              pointer to the API
   //--------------------------------------------------------------
   bool Request(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] api             pointer to the API
   /// \param[in] WUConfiguration The configuration of the module
   /// \param[in] PluginName      The name of the plugin module
   //--------------------------------------------------------------
   void Parse(boost::shared_ptr<yApi::IYPluginApi> api, const IWUConfiguration& WUConfiguration) const;

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] api                pointer to the API
   /// \param[in] WUConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void OnUpdate(boost::shared_ptr<yApi::IYPluginApi> api, IWUConfiguration& WUConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Return the conditions city name
   //--------------------------------------------------------------
   std::string GetCityName() const;

   //--------------------------------------------------------------
   /// \brief	  Return true if an error occured during the request
   /// \return    The state of this request
   //--------------------------------------------------------------
   bool IsModuleInFault() const;

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWeatherConditions();

private:

   //--------------------------------------------------------------
   /// \brief	  Initialise LiveConditions variables
   /// \param[in] api                pointer to the API
   /// \param[in] WUConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void InitializeVariables(boost::shared_ptr<yApi::IYPluginApi> api,
                            IWUConfiguration& WUConfiguration) const;

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
   CTemp m_Temp;

   //--------------------------------------------------------------
   /// \brief	The pressure (hPa)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CPressure> m_pressure;
   boost::shared_ptr<yApi::historization::CHumidity> m_humidity;
   boost::shared_ptr<yApi::historization::CDistance> m_visibility;
   boost::shared_ptr<yApi::historization::CDirection> m_uv;
   boost::shared_ptr<yApi::historization::CDirection> m_WindDirection;

   CTemp m_DewPoint;
   CRain m_Rain_1hr;
   CWeatherIcon m_WeatherConditionUrl;
   //CWindDirection  m_WindDirection;
   CWindSpeed m_WindAverageSpeed;
   CWindSpeed m_WindMaxSpeed;
   CTemp m_FeelsLike;
   CTemp m_Windchill;
   CCondition m_LiveConditions;

   //--------------------------------------------------------------
   /// \brief	    Error Detecting ?
   //--------------------------------------------------------------
   bool m_CatchError;
};

