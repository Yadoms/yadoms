#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "Keywords/Temp.h"
#include "Keywords/Barometric.h"
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
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CForecastReceiver(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string APIKey, std::string Localisation);

   void Request( boost::shared_ptr<yApi::IYPluginApi> context );

   void Parse( boost::shared_ptr<yApi::IYPluginApi> context );

   void OnUpdate( std::string PluginName, std::string APIKey, std::string Localisation );

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CForecastReceiver();

private:
   std::string m_APIKey;

   std::string m_Localisation;

   std::string m_PluginName;

   //--------------------------------------------------------------
   /// \brief	    Raw Web Data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   std::stringstream m_URL;

   shared::CHttpMethods m_webServer;

   WUCapacities::CTemp m_Temp_C;
   WUCapacities::CTemp m_Temp_F;
   WUCapacities::CBarometric m_Pressure;

   std::string m_TemperatureFilter;
};

