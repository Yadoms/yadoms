#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/xpl/XplMessage.h>
#include <shared/xpl/XplService.h>
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
   virtual void doWork(const std::string& configurationValues, boost::asio::io_service * pluginIOService);
   virtual void updateConfiguration(const std::string& configurationValues);
  // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief	TODO : commenter
   //--------------------------------------------------------------
   // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
   void init(const std::string & pluginAddress, const std::string & serverAddress);

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CFakePluginConfiguration m_Configuration;
};



