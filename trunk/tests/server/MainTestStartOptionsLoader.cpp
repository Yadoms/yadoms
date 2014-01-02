#define BOOST_AUTO_TEST_MAIN
#include "stdafx.h"
/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/

#include <iostream>
#include <fstream>
//#include "../../sources/server/Supervisor.h"
#include "../../sources/server/StartupOptionsLoader.h"
#include "../../sources/server/ApplicationStopHandler.h"
//#include "../../sources/tools/tools/Log.h"
/*
#include <fstream>
#include <boost/test/results_reporter.hpp>

//--------------------------------------------------------------
/// \brief	    Redirect in an xml file the result
//--------------------------------------------------------------

std::ofstream out;

struct ReportRedirector
{
    ReportRedirector()
    {
        out.open("test_results.xml");
        assert( out.is_open() );
        boost::unit_test::results_reporter::set_stream(out);
    }
};

BOOST_GLOBAL_FIXTURE(ReportRedirector)
*/
//--------------------------------------------------------------
/// \brief	    This function is needed to check the correct exception
//--------------------------------------------------------------

bool validate(CStartupOptionsLoaderException str)
{
 return str.isError();
}

/*
\brief Main application entry point
*/

/*void write_settings ( std::string value )
{
  std::ofstream settings_file ("yadoms.cfg");

  //settings_file << "port = " << value.c_str();
  settings_file << "port = 8085";
  settings_file.close();
}
*/

void write_settings ( void )
{
  std::ofstream settings_file ("yadoms.cfg");

  //settings_file << "port = " << value.c_str();
  settings_file << "port = 8085";
  settings_file.close();
}

#define BOOST_TEST_MODULE TestStartupOptionsLoader

BOOST_AUTO_TEST_SUITE(Initialisation)

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with no argument
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Initialisation_Test)
{
  CStartupOptionsLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the change of the port with -p
/// \result         No Error - port number change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Port_p_Initialisation)
{ 
  char *argv[] = {"./MainTestStartOptionsLoader","-p","2000"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 2000);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the change of the port with --port
/// \result         No Error - port number change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Port_port_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--port","2000"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 2000);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with an argument error : --por
/// \result         Error : Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Port_Initialisation_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--por","2000"};

  //CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );
/*
  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
*/
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with an argument error : -port
/// \result         Error : Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Port_Initialisation_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-port","2000"};
  
  //Test the exception, and if this one is the correct one !
  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );

  //BOOST_REQUIRE_THROW(CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderError);
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument --databaseFile
/// \result         No Error - database name change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--databaseFile","toto.db3"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the faulty argument --database
/// \result         Error : Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--database","toto.db3"};

//  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );

/*
  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
*/
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -d
/// \result         No Error - database name change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_d_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-D","toto.db3"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -l
/// \result         No Error - logs change to trace
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_trace_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","trace"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::trace);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -l
/// \result         No Error - logs change to debug
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_debug_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","debug"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::debug);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -l
/// \result         No Error - logs change to info
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_info_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","info"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -l
/// \result         No Error - logs change to warning
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_warning_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","warning"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -l
/// \result         No Error - logs change to error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_error_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","error"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::error);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -l
/// \result         No Error - logs change to fatal
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_fatal_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","fatal"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::fatal);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with a faulty log argument -l
/// \result         Raise an exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Unknow_Log_l_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","toto"};

  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with a faulty argument entry
/// \result         Raise an exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Unknow_option_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-a","info"};

  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -i
/// \result         No Error - the adresse IP is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_i_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-i","192.168.1.1"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.1");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -i
/// \result         No Error - the adresse IP is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--webServerIp","192.168.1.1"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.1");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the wrong argument -webServe
/// \result         Raise a Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--webServe","192.168.1.1"};

  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with a malform IP address (not complete)
/// \result         Raise a Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-i","192.168.1."};

  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -w
/// \result         No Error - the website address is changed
//--------------------------------------------------------------
// TODO : Create a directory compatible for every platform
BOOST_AUTO_TEST_CASE(Different_WebServer_w_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-w","/home"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "/home");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -w
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--webServerPath","/home/"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "/home/");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -w
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_WrongPath)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--webServerPath","/home/ww"};

//  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_REQUIRE_EXCEPTION( CStartupOptionsLoader StartupOptions (3, argv), CStartupOptionsLoaderException, validate );
/*
  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "/home/ww");
*/
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -w
/// \result         No Error - the hardware plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_HardwarePluginsPath_h_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-h","/home/"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"/home/");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -w
/// \result         No Error - the hardware plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_HardwarePluginsPath_hardwarePluginsPath_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--hardwarePluginsPath","/home/"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"/home/");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -d
/// \result         No Error - the device plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_DevicePluginsPath_d_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-d","/home/"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"/home/");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -DevicePluginsPath
/// \result         No Error - the device plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_DevicePluginsPath_devicePluginsPath_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--devicePluginsPath","/home/"};

  CStartupOptionsLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"/home/");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -x
/// \result         No Error - the XPL is disactivated
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Disable_XPL_x_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-x"};

  CStartupOptionsLoader StartupOptions (2, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with the argument -disableXplHubStart
/// \result         No Error - the XPL is disactivated
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Disable_XPL_disableXplHubStart_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--disableXplHubStart"};

  CStartupOptionsLoader StartupOptions (2, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

BOOST_AUTO_TEST_CASE(Toto3)
{
  //Creation of the config file
  write_settings();

  CStartupOptionsLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8085);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

BOOST_AUTO_TEST_SUITE_END()


