#include "HardwarePluginBase.h"

IMPLEMENT_HARDWARE_PLUGIN(CFakePlugin)

namespace plugins
{
   class CFakePlugin : public IHardwarePlugin
   {  
   public:
      CFakePlugin()
      {
      }

      virtual ~CFakePlugin()
      {
      }

      // IHardwarePlugin implementation
      virtual void Start()
      {
         std::cout << "Start CFakePlugin" << std::endl;
      }

      virtual void Stop()
      {
         std::cout << "Stop CFakePlugin" << std::endl;
      }

      virtual void DoWork()
      {
         while(1)
         {
            // Give a chance to exit plugin thread
            boost::this_thread::sleep(boost::posix_time::seconds(1000)); 

            std::cout << "CFakePlugin is running..." << std::endl;
         };
      }
      // [END] IHardwarePlugin implementation

      // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
      void init(const std::string & pluginAddress, const std::string & serverAddress)
      {
      }
   };
}


