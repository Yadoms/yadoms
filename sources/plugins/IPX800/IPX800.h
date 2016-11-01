#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "IPX800Configuration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the IPX800 plugin
//--------------------------------------------------------------
class CIPX800 : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CIPX800();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CIPX800();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief Send a command to the IPX800
   /// \param[in] IPAddress             IP Address of the equipment
   /// \param[in] M2MPassword           password of the equipment
   //--------------------------------------------------------------
   void SendCommand(Poco::Net::IPAddress IPAddress, std::string M2MPassword);

private:
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief Configuration of the device
   //--------------------------------------------------------------
   CIPX800Configuration m_configuration;
};

