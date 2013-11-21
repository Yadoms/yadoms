#pragma once

#include "../IHardwarePluginImplementationHelper.h"

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

IMPLEMENT_HARDWARE_PLUGIN(CFakePlugin,"FakePlugin","0.1",plugins::CHardwarePluginInformation::kBeta,"[yadoms-team](http://sourceforge.net/projects/yadoms/)","")
