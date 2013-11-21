#include "FakePlugin.h"

#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>

namespace plugins
{
	  CFakePlugin::CFakePlugin()
      {
      }

      CFakePlugin::~CFakePlugin()
      {
      }

      // IHardwarePlugin implementation
      
      void CFakePlugin::doWork()
      {
         while(1)
         {
            BOOST_LOG_TRIVIAL(info) << "CFakePlugin is running...";

            // Give a chance to exit plugin thread
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); 
         };
      }

	  // [END] IHardwarePlugin implementation


      // TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
      void CFakePlugin::init(const std::string & pluginAddress, const std::string & serverAddress)
      {
      }
}
