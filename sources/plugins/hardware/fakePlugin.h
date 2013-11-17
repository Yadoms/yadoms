#pragma once

#include "IHardwarePlugin.h"
#include <string>

namespace plugins
{
   class CFakePlugin : public IHardwarePlugin
   {  
   public:
      CFakePlugin();

      virtual ~CFakePlugin();

      // IHardwarePlugin implementation
      /*virtual void start();

      virtual void stop();*/

      virtual void doWork();

	  //Return the information about the class under string format
	  virtual std::string toString();
	  // [END] IHardwarePlugin implementation

      // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
      void init(const std::string & pluginAddress, const std::string & serverAddress);
   };
}

IMPLEMENT_HARDWARE_PLUGIN(CFakePlugin)