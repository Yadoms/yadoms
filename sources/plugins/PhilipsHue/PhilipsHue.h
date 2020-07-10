#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "Configuration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is an empty plugin example
/// \note   This plugin do nothing, you just have to :
///         - rename all classes of this plugin
///         - populate all code/functions
//--------------------------------------------------------------
class CPhilipsHue : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPhilipsHue();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPhilipsHue();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   Configuration m_configuration;
};