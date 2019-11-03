#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/yPluginApi/YPluginConfiguration.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

enum EPortType
{
   kHttp = 8080,
   kHttps = 4343
};

//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class CLametricConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CLametricConfiguration() = default;

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in]   data The data container
   //--------------------------------------------------------------
   void initializeWith(const shared::CDataContainer& data);

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace() const;
   /**
    * \brief  Get Device IP address
    * \return Returns Device IP address
    */
   std::string getIPAddress() const;
   /**
    * \brief  Get port
    * \return Returns port
    */
   EPortType getPort() const;
   /**
    * \brief  Get Device Api Key 
    * \return Returns Device Api Key
    */
   std::string getAPIKey() const;

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration getter
   //--------------------------------------------------------------
   yApi::YPluginConfiguration m_configuration;
};
