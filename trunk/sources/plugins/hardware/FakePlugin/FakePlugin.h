#pragma once

#include "../HardwarePluginImplementationHelper.h"

#include <string>

class CFakePlugin : public IHardwarePlugin
{  
public:
   CFakePlugin();

   virtual ~CFakePlugin();

   // IHardwarePlugin implementation
   virtual void doWork(const IHardwarePluginConfigurationProvider& config);
  // [END] IHardwarePlugin implementation

   // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
   void init(const std::string & pluginAddress, const std::string & serverAddress);
};



