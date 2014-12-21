#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "Keywords/Temp.h"
#include "Keywords/Barometric.h"
#include "Keywords/UV.h"
#include "IWUConfiguration.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	ForecastReader Module
/// \note   This class consist of the send and the received of the forecast information from the web
//--------------------------------------------------------------
class CForecastReceiver
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] context       pointer to the API
   /// \param[in] PluginName    The Plugin Name
   /// \param[in] APIKey        The API Key
   /// \param[in] Localisation  The Localisation
   //--------------------------------------------------------------
   CForecastReceiver(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string APIKey, std::string Localisation);

   //--------------------------------------------------------------
   /// \brief	  Send the request and receive the response from the web site
   /// \param[in] context    pointer to the API
   //--------------------------------------------------------------
   void Request( boost::shared_ptr<yApi::IYPluginApi> context );

   //--------------------------------------------------------------
   /// \brief	  Parse the answer from the web Site
   /// \param[in] context    pointer to the API
   //--------------------------------------------------------------
   void Parse( boost::shared_ptr<yApi::IYPluginApi> context );

   //--------------------------------------------------------------
   /// \brief	  Update the configuration when something change from the HMI
   /// \param[in] PluginName    The Plugin Name
   /// \param[in] APIKey        The API Key
   /// \param[in] Localisation  The Localisation
   //--------------------------------------------------------------
   void OnUpdate( std::string PluginName, std::string APIKey, std::string Localisation );

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CForecastReceiver();

private:

   //--------------------------------------------------------------
   /// \brief	    The API Key for access the web site
   //--------------------------------------------------------------
	std::string m_APIKey;

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_Localisation;

   //--------------------------------------------------------------
   /// \brief	    The PluginName
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
   /// \brief	    The local temperature in Celcius
   //--------------------------------------------------------------
   WUCapacities::CTemp m_Temp_C;

   //--------------------------------------------------------------
   /// \brief	    The local temperature in Fahrenheit
   //--------------------------------------------------------------
   WUCapacities::CTemp m_Temp_F;

   //--------------------------------------------------------------
   /// \brief	    The Windchill temperature in Celcius
   //--------------------------------------------------------------
   WUCapacities::CTemp m_Windchill_C;

   //--------------------------------------------------------------
   /// \brief	    The Windchill temperature in Fahrenheit
   //--------------------------------------------------------------
   WUCapacities::CTemp m_Windchill_F;

   //--------------------------------------------------------------
   /// \brief	    The DewPoint temperature in Celcius
   //--------------------------------------------------------------
   WUCapacities::CTemp m_DewPoint_C;

   //--------------------------------------------------------------
   /// \brief	    The DewPoint temperature in Fahrenheit
   //--------------------------------------------------------------
   WUCapacities::CTemp m_DewPoint_F;

   //--------------------------------------------------------------
   /// \brief	    The pressure
   //--------------------------------------------------------------
   WUCapacities::CBarometric m_Pressure;

   //--------------------------------------------------------------
   /// \brief	    The UV exposure
   //--------------------------------------------------------------
   WUCapacities::CUV m_UV;
};

