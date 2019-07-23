#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "OledEspEasyConfiguration.h"
#include "OledEspEasyController.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is an empty plugin example
/// \note   This plugin do nothing, you just have to :
///         - rename all classes of this plugin
///         - populate all code/functions
//--------------------------------------------------------------
class COledEspEasy : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   COledEspEasy();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~COledEspEasy();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   COledEspEasyConfiguration m_configuration;

   boost::shared_ptr<COledEspEasyController> m_controller;
};