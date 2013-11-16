#include "HardwarePluginBase.h"
#include "FakePlugin.h"

#include <boost\thread.hpp>

namespace plugins
{
	  CFakePlugin::CFakePlugin()
      {
      }

      CFakePlugin::~CFakePlugin()
      {
      }

      // IHardwarePlugin implementation
      /*void CFakePlugin::start()
      {
         std::cout << "Start CFakePlugin" << std::endl;
      }

      void CFakePlugin::stop()
      {
         std::cout << "Stop CFakePlugin" << std::endl;
      }*/

      void CFakePlugin::doWork()
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
      void CFakePlugin::init(const std::string & pluginAddress, const std::string & serverAddress)
      {
      }
}
