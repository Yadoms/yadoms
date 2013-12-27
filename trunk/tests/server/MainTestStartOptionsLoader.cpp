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

bool validate(CStartupOptionsLoaderError str)
{
 return str.isError();
}

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
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}


BOOST_AUTO_TEST_CASE(Different_Port_p_Initialisation)
{ 
  char *argv[] = {"./MainTestStartOptionsLoader","-p","2000"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 2000);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Port_port_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--port","2000"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 2000);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Port_Initialisation_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--por","2000"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Port_Initialisation_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-port","2000"};
  
  //Test the exception, and if this one is the correct one !
  BOOST_CHECK_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderError, validate );

  //BOOST_REQUIRE_THROW(CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderError);
}

BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--databaseFile","toto.db3"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--database","toto.db3"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Database_d_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-d","toto.db3"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Log_l_trace_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","trace"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::trace);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Log_l_debug_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","debug"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::debug);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Log_l_info_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","info"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Log_l_warning_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","warning"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Log_l_error_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","error"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::error);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Different_Log_l_fatal_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","fatal"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::fatal);
  BOOST_CHECK_EQUAL(StartupOptions.getPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
}

BOOST_AUTO_TEST_CASE(Unknow_Log_l_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","toto"};

  BOOST_CHECK_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderError, validate );
}

BOOST_AUTO_TEST_CASE(Unknow_option_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-a","toto"};

  BOOST_CHECK_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderError, validate );
}

BOOST_AUTO_TEST_SUITE_END()


