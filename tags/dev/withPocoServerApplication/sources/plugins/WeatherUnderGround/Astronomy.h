#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include <shared/http/HttpMethods.h>
#include "WUConfiguration.h"
#include "Keywords/Load.h"
#include "Keywords/Duration.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Astronomy Module
/// \note   This class consist of the traitment of the astronomy information from the web site
//--------------------------------------------------------------
class CAstronomy
{
public:
   //--------------------------------------------------------------
   /// \brief	  Constructor
   /// \param[in] context          pointer to the API
   /// \param[in] WUConfiguration  The Configuration of the module
   //--------------------------------------------------------------
   CAstronomy(boost::shared_ptr<yApi::IYPluginApi> context, const IWUConfiguration& WUConfiguration, const std::string & PluginName, const std::string & Prefix);

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
   void OnUpdate( const IWUConfiguration& WUConfiguration );

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CAstronomy();

private:

   //--------------------------------------------------------------
   /// \brief	    Your Location to received custom information from the web site
   //--------------------------------------------------------------
   std::string m_Localisation;

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
   CLoad     PercentIlluminatedMoon;
   CDuration AgeOfMoon;
};

