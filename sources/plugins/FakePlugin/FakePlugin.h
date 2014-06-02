#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/event/EventHandler.hpp>
#include "FakePluginConfiguration.h"

//--------------------------------------------------------------
/// \brief	This class is a simple plugin example
/// \note   This plugin do nothing useful, just demonstrate how to :
///         - use a configuration
///         - receive a XPL message
///         - send a XPL message
//--------------------------------------------------------------
class CFakePlugin : public shared::event::CEventHandler, public shared::plugin::IPlugin
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
   virtual void doWork(boost::shared_ptr<shared::plugin::yadomsApi::IYadomsApi> yadoms);
   virtual void updateConfiguration(const std::string& configuration);
  // [END] IPlugin implementation

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CFakePluginConfiguration m_configuration;
};



