#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include <shared/event/EventTimer.h>
#include "OrangeBusinessConfiguration.h"
#include "urlManager.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the IPX800 plugin
//--------------------------------------------------------------
class COrangeBusiness : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   COrangeBusiness();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~COrangeBusiness();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] neMessage             The new message to be processed
   //--------------------------------------------------------------
   void processRetreiveData(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] newConfigurationData  The new configuration of the module
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

private:

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] neMessage             The new message to be processed
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceName) const;

   //--------------------------------------------------------------
   /// \brief Configuration of the device
   //--------------------------------------------------------------
   COrangeBusinessConfiguration m_configuration;

   urlManager m_frameManager;

   //--------------------------------------------------------------
   /// \brief developer mode for logs
   //--------------------------------------------------------------
   bool m_isDeveloperMode;
};