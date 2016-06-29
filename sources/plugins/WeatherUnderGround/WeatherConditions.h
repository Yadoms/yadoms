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
   /// \param[in] wuConfiguration  The Configuration of the module
   //--------------------------------------------------------------
   CWeatherConditions(boost::shared_ptr<yApi::IYPluginApi> api,
                      IWUConfiguration& wuConfiguration,
                      const std::string& pluginName,
                      const std::string& prefix);

   //--------------------------------------------------------------
   /// \brief	  Send the request and receive the response from the web site
   /// \param[in] api              pointer to the API
   //--------------------------------------------------------------
   bool request(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] api             pointer to the API
   /// \param[in] wuConfiguration The configuration of the module
   /// \param[in] pluginName      The name of the plugin module
   //--------------------------------------------------------------
   void parse(boost::shared_ptr<yApi::IYPluginApi> api,
              const IWUConfiguration& wuConfiguration) const;

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] api                pointer to the API
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void onUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                 IWUConfiguration& wuConfiguration);

   //--------------------------------------------------------------
   /// \brief	  Return the conditions city name
   //--------------------------------------------------------------
   std::string getCityName() const;

   //--------------------------------------------------------------
   /// \brief	  Return true if an error occured during the request
   /// \return    The state of this request
   //--------------------------------------------------------------
   bool isModuleInFault() const;

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWeatherConditions();

private:

   //--------------------------------------------------------------
   /// \brief	  Initialise LiveConditions variables
   /// \param[in] api                pointer to the API
   /// \param[in] wuConfiguration    The Plugin configuration
   //--------------------------------------------------------------
   void initializeVariables(boost::shared_ptr<yApi::IYPluginApi> api,
                            IWUConfiguration& wuConfiguration) const;

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_localisation;

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_countryOrState;

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
   /// \brief	    The name of the city (country ? state ?)
   //--------------------------------------------------------------
   std::string m_cityConditions;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   CTemp m_temp;

   //--------------------------------------------------------------
   /// \brief	The pressure (hPa)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CPressure> m_pressure;
   boost::shared_ptr<yApi::historization::CHumidity> m_humidity;
   boost::shared_ptr<yApi::historization::CDistance> m_visibility;
   boost::shared_ptr<yApi::historization::CDirection> m_uv;
   boost::shared_ptr<yApi::historization::CDirection> m_WindDirection;

   CTemp m_dewPoint;
   CRain m_rain1hr;
   CWeatherIcon m_weatherConditionUrl;
   CWindSpeed m_windAverageSpeed;
   CWindSpeed m_windMaxSpeed;
   CTemp m_feelsLike;
   CTemp m_windchill;
   CCondition m_liveConditions;

   //--------------------------------------------------------------
   /// \brief	    Error Detected ?
   //--------------------------------------------------------------
   bool m_catchError;
};

