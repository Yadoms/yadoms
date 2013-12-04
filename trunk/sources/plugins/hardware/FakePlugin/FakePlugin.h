#pragma once

#include "../HardwarePluginImplementationHelper.h"

#include <string>
#include <boost/thread/future.hpp>

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
   boost::optional<boost::shared_ptr<const CHardwarePluginConfiguration> > getUpdatedConfiguration();   // TODO à mettre dans une macro DECLARE_CONFIGURATION
   
   // TODO à mettre dans une macro DECLARE_CONFIGURATION
   std::queue<boost::shared_ptr<const CHardwarePluginConfiguration> > m_ConfigurationUpdateQueue;
   boost::mutex m_configurationUpdateMutex;
};



