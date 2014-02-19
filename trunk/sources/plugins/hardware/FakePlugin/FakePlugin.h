#pragma once
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Xpl/XplMessage.h>
#include <shared/Xpl/XplService.h>
#include <shared/Event/EventHandler.hpp>
#include "FakePluginConfiguration.h"

class CFakePlugin : public CEventHandler, public IHardwarePlugin, public boost::enable_shared_from_this<CFakePlugin>

{  
public:
   CFakePlugin();

   virtual ~CFakePlugin();

   // IHardwarePlugin implementation
   virtual void doWork(const std::string& configurationValues, boost::shared_ptr< boost::asio::io_service > pluginIOService);
   virtual void updateConfiguration(const std::string& configurationValues);
  // [END] IHardwarePlugin implementation

   // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
   void init(const std::string & pluginAddress, const std::string & serverAddress);

private:
   void onMessageReceived(CXplMessage & message);

   // Trace the configuration
   // This function shows how to get configuration values
   void traceConfiguration();

   CFakePluginConfiguration m_Configuration;
};



