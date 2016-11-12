#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "WUConfiguration.h"
#include "Keywords/WeatherIcon.h"
#include "Keywords/Condition.h"

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
                      const std::string& deviceName,
                      const std::string& prefix);

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] api             pointer to the API
   /// \param[in] wuConfiguration The configuration of the module
   /// \param[in] pluginName      The name of the plugin module
   //--------------------------------------------------------------
   void parse(boost::shared_ptr<yApi::IYPluginApi> api,
              const IWUConfiguration& wuConfiguration,
              const shared::CDataContainer dataToParse);

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
   /// \brief	  Return the url
   /// \return    The url string
   //--------------------------------------------------------------
   std::string getUrl() const;

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
   /// \brief	    The Device Name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	    The url link to access properly the web site
   //--------------------------------------------------------------
   std::stringstream m_url;

   //--------------------------------------------------------------
   /// \brief	    The name of the city (country ? state ?)
   //--------------------------------------------------------------
   std::string m_cityConditions;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temp;
   boost::shared_ptr<yApi::historization::CPressure> m_pressure;
   boost::shared_ptr<yApi::historization::CHumidity> m_humidity;
   boost::shared_ptr<yApi::historization::CDistance> m_visibility;
   boost::shared_ptr<yApi::historization::CDirection> m_uv;
   boost::shared_ptr<yApi::historization::CDirection> m_WindDirection;
   boost::shared_ptr<yApi::historization::CTemperature> m_dewPoint;
   boost::shared_ptr<yApi::historization::CRainRate> m_rain1hr;
   boost::shared_ptr<CWeatherIcon> m_weatherConditionUrl;
   boost::shared_ptr<yApi::historization::CSpeed> m_windAverageSpeed;
   boost::shared_ptr<yApi::historization::CSpeed> m_windMaxSpeed;
   boost::shared_ptr<yApi::historization::CTemperature> m_feelsLike;
   boost::shared_ptr<yApi::historization::CTemperature> m_windchill;
   boost::shared_ptr<CCondition> m_liveConditions;

   //--------------------------------------------------------------
   /// \brief  Keywords list
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;

};