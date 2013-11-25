#include "FakePlugin.h"

#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>

CFakePlugin::CFakePlugin()
{
}

CFakePlugin::~CFakePlugin()
{
}

// IHardwarePlugin implementation

void CFakePlugin::doWork()
{
   try
   {
	   while(1)
	   {
	      BOOST_LOG_TRIVIAL(info) << "CFakePlugin is running...";
	
	      // Give a chance to exit plugin thread
	      boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); 
	   };
   }
   // Plugin must catch this end-of-thread exception
   catch (boost::thread_interrupted& e)
   {
      BOOST_LOG_TRIVIAL(info) << "CFakePlugin is stopped...";
   }
}

// [END] IHardwarePlugin implementation


// TODO : WhatTheFuck ? ? ? C'est quoi ces adresses ?
void CFakePlugin::init(const std::string & pluginAddress, const std::string & serverAddress)
{
}

