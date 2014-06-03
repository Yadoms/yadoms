#pragma once
#include <shared/plugin/IPlugin.h>
#include "FakePluginConfiguration.h"

//--------------------------------------------------------------
/// \brief	This class is a simple plugin example
/// \note   This plugin do nothing useful, just demonstrate how to :
///         - use a configuration
///         - process a command from Yadoms
///         - send sensor state to Yadoms
//--------------------------------------------------------------
class CFakePlugin : public shared::plugin::IPlugin
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
   virtual void doWork(boost::shared_ptr<shared::plugin::yadomsApi::IYadomsApi> yApi);
  // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CFakePluginConfiguration m_configuration;
};



