#define BOOST_AUTO_TEST_MAIN
#include "stdafx.h"
/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/

#include <iostream>
//#include "../../sources/server/Supervisor.h"
#include "../../sources/server/StartupOptionsLoader.h"
#include "../../sources/server/ApplicationStopHandler.h"
//#include "../../sources/tools/tools/Log.h"


/*
\brief Main application entry point
*/

#define BOOST_TEST_MODULE TestStartupOptionsLoader

BOOST_AUTO_TEST_SUITE(Initialisation)

BOOST_AUTO_TEST_CASE(Initialisation_Test)
{
  CStartupOptionsLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
}

BOOST_AUTO_TEST_CASE(Initialisation_Error1)
{
  CStartupOptionsLoader StartupOptions (5, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
}

BOOST_AUTO_TEST_SUITE_END()

/*
int main (int argc, char** argv)
{

   CApplicationStopHandler::configure();

   try
   {
      CStartupOptionsLoader startupOptions(argc, argv);

      CLog::configure(startupOptions.getLogLevel());
      YADOMS_LOG_CONFIGURE("Main");
      
      YADOMS_LOG(info) << "********************************************************************";
      YADOMS_LOG(info) << "Yadoms is starting";
      YADOMS_LOG(info) << "********************************************************************";
      YADOMS_LOG(info) << "Startup options :";
      YADOMS_LOG(info) << "\tlog level = " << startupOptions.getLogLevel();
      YADOMS_LOG(info) << "\tport number = " << startupOptions.getPortNumber();
      YADOMS_LOG(info) << "\tdb path = " << startupOptions.getDatabaseFile();
      YADOMS_LOG(info) << "\thardware plugins = " << startupOptions.getHarwarePluginsPath();
      YADOMS_LOG(info) << "\tdevice plugins = " << startupOptions.getDevicePluginsPath();
      YADOMS_LOG(info) << "\tstart Xpl Hub = " << startupOptions.getStartXplHubFlag();
      YADOMS_LOG(info) << "********************************************************************";


      CSupervisor supervisor(startupOptions);
      supervisor.start();

      while(!CApplicationStopHandler::stopRequested())
      {
         boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }

      supervisor.stop();
      
      YADOMS_LOG(info) << "Yadoms is stopped ";
   }
   catch(CStartupOptionsLoaderError& e)
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
      std::cout << "An unhandled exception occurs. Yadoms is now stopped" << std::endl;
      YADOMS_LOG(fatal) << "An unhandled exception occurs. Yadoms is now stopped";
   }


   return 0;
}
*/
