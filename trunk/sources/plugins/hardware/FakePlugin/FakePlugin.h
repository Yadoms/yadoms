#pragma once
#include <shared/HardwarePlugin/ImplementationHelper.h>
#include <shared/Xpl/XplMessage.h>

class CFakePlugin : public IHardwarePlugin
{  
public:
   CFakePlugin();

   virtual ~CFakePlugin();

   // IHardwarePlugin implementation
   virtual void doWork(const std::string& configurationValues);
   virtual void updateConfiguration(const std::string& configurationValues);
  // [END] IHardwarePlugin implementation

   // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
   void init(const std::string & pluginAddress, const std::string & serverAddress);

private:
   std::string getUpdatedConfiguration();   // TODO à mettre dans une macro DECLARE_CONFIGURATION
   
   // TODO à mettre dans une macro DECLARE_CONFIGURATION
   std::queue<std::string> m_ConfigurationUpdateQueue;
   boost::mutex m_configurationUpdateMutex;

   void onMessageReceived(CXplMessage & message);

   // Trace the configuration
   void traceConfiguration(const CHardwarePluginConfiguration& configuration);
};



