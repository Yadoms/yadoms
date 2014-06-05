#include "stdafx.h"
/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/

#include <iostream>
#include "Supervisor.h"
#include "startupOptions/Loader.h"
#include <shared/Log.h>


/*
\brief Main application entry point
*/
int main (int argc, char** argv)
{
   try
   {
      startupOptions::CLoader startupOptions(argc, argv);

      if(startupOptions.getDebugFlag())
         shared::CLog::configure_file_per_thread(startupOptions.getLogLevel());
      else
         shared::CLog::configure_one_rolling_file(startupOptions.getLogLevel());

      YADOMS_LOG_CONFIGURE("Main");

      YADOMS_LOG(info) << "********************************************************************";
      YADOMS_LOG(info) << "Yadoms is starting";
      YADOMS_LOG(info) << "********************************************************************";
      YADOMS_LOG(info) << "Startup options :";
      YADOMS_LOG(info) << "\tlog level = " << startupOptions.getLogLevel();
      YADOMS_LOG(info) << "\tWeb server port number = " << startupOptions.getWebServerPortNumber();
      YADOMS_LOG(info) << "\tWeb server ip = " << startupOptions.getWebServerIPAddress();
      YADOMS_LOG(info) << "\tWeb server path = " << startupOptions.getWebServerInitialPath();
      YADOMS_LOG(info) << "\tdb path = " << startupOptions.getDatabaseFile();
      YADOMS_LOG(info) << "\tplugins path = " << startupOptions.getPluginsPath();
      YADOMS_LOG(info) << "\twidgets path = " << startupOptions.getWidgetsPath();
      YADOMS_LOG(info) << "********************************************************************";


      CSupervisor supervisor(startupOptions);

      // The main job
      supervisor.doWork();

      YADOMS_LOG(info) << "Yadoms is stopped ";
   }
   catch(startupOptions::CLoaderException& e)
   {
      if (e.isError())
      {
         YADOMS_LOG(fatal) << e.what();
      }
      else
      {
         // Help was invoked, just print to console
         YADOMS_LOG(info) << e.what();
      }
   }
   catch(...)
   {
      //dual logging in case logger fails/throws
      std::cout << "An unhandled exception occurred. Yadoms is now stopped" << std::endl;
      YADOMS_LOG(fatal) << "An unhandled exception occurred. Yadoms is now stopped";
      BOOST_ASSERT_MSG(false, "Yadoms exited with error, see console for details");
   }

   //flush the log before exit
   boost::log::core::get()->flush();

   return 0;
}