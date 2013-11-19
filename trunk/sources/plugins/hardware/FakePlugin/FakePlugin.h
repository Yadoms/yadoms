#pragma once

#include "../IHardwarePlugin.h"
#include "../HardwarePluginInformation.h"

#include <string>

namespace plugins
{
   class CFakePlugin : public IHardwarePlugin
   {  
   public:
      CFakePlugin();

      virtual ~CFakePlugin();

      // IHardwarePlugin implementation
      virtual void doWork();
	  // [END] IHardwarePlugin implementation

      // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
      void init(const std::string & pluginAddress, const std::string & serverAddress);
   };
}

IMPLEMENT_HARDWARE_PLUGIN(CFakePlugin)

extern "C"                                                         
{                                                                      
   EXPORT_MAIN_FUNCTIONS plugins::CHardwarePluginInformation* getInfo()           
   {                                                                   
      return new plugins::CHardwarePluginInformation("FakePlugin", "1.0",  plugins::CHardwarePluginInformation::kBeta, "", "");                  
   }                                                                   
                                                                       
}   
