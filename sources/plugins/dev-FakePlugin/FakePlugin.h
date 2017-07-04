#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "FakePluginConfiguration.h"
#include "FakeDynamicallyConfigurableDevice.h"
#include "FakeAnotherConfigurableDevice.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is a simple plugin example
/// \note   This plugin do nothing useful, just demonstrate how to :
///         - use a configuration
///         - process a command from Yadoms
///         - send sensor state to Yadoms
//--------------------------------------------------------------
class CFakePlugin : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CFakePlugin();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CFakePlugin();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CFakePluginConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The managed devices
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<CFakeDynamicallyConfigurableDevice>> m_manuallyCreatedDynaConfDevices;
   std::vector<boost::shared_ptr<CFakeAnotherConfigurableDevice>> m_manuallyCreatedStaticConfDevices;
};

