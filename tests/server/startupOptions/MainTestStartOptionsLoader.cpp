// Includes needed to compile tested classes
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "../../../sources/server/startupOptions/Loader.h"
#include "../../../sources/server/ApplicationStopHandler.h"

#define BOOST_TEST_MODULE TestStartupOptionsLoader

#include <boost/test/unit_test.hpp>

//--------------------------------------------------------------
/// \brief	    This function is needed to check the correct exception
//--------------------------------------------------------------

bool validate(startupOptions::CLoaderException str)
{
 return str.isError();
}

void write_settings ( std::string name, std::string value )
{
	std::ofstream settings_file;

	settings_file.open( "yadoms.cfg", std::fstream::out | std::fstream::app );

	settings_file << name.c_str() << " = " << value.c_str() << "\n";
	settings_file.close();
}

void CreateDirectory ( std::string name )
{
   boost::filesystem::path dir(name.c_str());
   BOOST_TEST_REQUIRE(boost::filesystem::create_directory(dir));
}

void RemoveDirectory (std::string name )
{
	boost::filesystem::remove_all(name.c_str());
}

void RemoveFile ( std::string name)
{
	if(boost::filesystem::exists(name.c_str()))
	                boost::filesystem::remove(name.c_str());
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with no argument
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Initialisation_Test)
{
	//remove the configuration before tests
	RemoveFile ("yadoms.cfg");

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the change of the port with -p
/// \result         No Error - port number change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Port_p_Initialisation)
{ 
  char *argv[] = {"./MainTestStartOptionsLoader","-p","2000"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)2000);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the change of the port with --port
/// \result         No Error - port number change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Port_port_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--port","2000"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)2000);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with an argument error : --por
/// \result         Error : Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Port_Initialisation_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--por","2000"};

  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with an argument error : -port
/// \result         Error : Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Port_Initialisation_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-port","2000"};
  
  //Test the exception, and if this one is the correct one !
  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument --databaseFile
/// \result         No Error - database name change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--databaseFile","toto.db3"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the faulty argument --database
/// \result         Error : Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--database","toto.db3"};

  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -d
/// \result         No Error - database name change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_d_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-D","toto.db3"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -l
/// \result         No Error - logs change to trace
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_trace_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","trace"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::trace);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -l
/// \result         No Error - logs change to debug
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_debug_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","debug"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::debug);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -l
/// \result         No Error - logs change to info
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_info_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","info"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -l
/// \result         No Error - logs change to warning
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_warning_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","warning"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -l
/// \result         No Error - logs change to error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_error_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","error"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::error);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -l
/// \result         No Error - logs change to fatal
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Log_l_fatal_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","fatal"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::fatal);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with a faulty log argument -l
/// \result         Raise an exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Unknow_Log_l_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-l","toto"};

  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with a faulty argument entry
/// \result         Raise an exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Unknow_option_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-a","info"};

  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -i
/// \result         No Error - the adresse IP is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_i_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-i","192.168.1.1"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.1");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -i
/// \result         No Error - the adresse IP is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--webServerIp","192.168.1.1"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.1");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the wrong argument -webServe
/// \result         Raise a Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Error1)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--webServe","192.168.1.1"};

  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with a malform IP address (not complete)
/// \result         Raise a Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Error2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-i","192.168.1."};

  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -w
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_w_Initialisation)
{
  CreateDirectory ("toto");

  char *argv[] = {"./MainTestStartOptionsLoader","-w","toto"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "toto");

  RemoveDirectory ("www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -w
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_Initialisation)
{
  CreateDirectory ("home");

  char *argv[] = {"./MainTestStartOptionsLoader","--webServerPath","home"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "home");

  RemoveDirectory ("home");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -w
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_WrongPath)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--webServerPath","/home/ww"};

  BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -w
/// \result         No Error - the hardware plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_HardwarePluginsPath_h_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-h","/home/"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"/home/");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -w
/// \result         No Error - the hardware plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_HardwarePluginsPath_hardwarePluginsPath_Initialisation)
{
  CreateDirectory ("home");

  char *argv[] = {"./MainTestStartOptionsLoader","--hardwarePluginsPath","home"};

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"home");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  RemoveDirectory ("home");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -DevicePluginsPath
/// \result         No Error - the device plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WidgetPath_WidgetPath_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--widgetsPath","home"};

  CreateDirectory ("home");

  startupOptions::CLoader StartupOptions (3, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"home");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  RemoveDirectory ("home");
}

//TODO : Tester HardwarePluginsPath et DevicePluginsPath avec un faux chemin

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -x
/// \result         No Error - the XPL is disactivated
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Disable_XPL_x_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-x"};

  startupOptions::CLoader StartupOptions (2, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -disableXplHubStart
/// \result         No Error - the XPL is disactivated
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Disable_XPL_disableXplHubStart_Initialisation)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--disableXplHubStart"};

  startupOptions::CLoader StartupOptions (2, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument port in configuration file
/// \result         No Error - the port number is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_Port_Number)
{
  //Creation of the config file
  write_settings( "port", "8085" );

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument port in configuration file
/// \result         No Error - the port number is not changed - No Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_Port_Number_Error1)
{
  //Creation of the config file
  write_settings( "portable", "8085" );

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  // There should no have exception !
  BOOST_REQUIRE_NO_THROW( startupOptions::CLoader StartupOptions (0, NULL) );

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument databaseFile in configuration file
/// \result         No Error - the database file name is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_database_file)
{
  //Creation of the config file
  write_settings( "databaseFile", "AnOtherFile.db3" );

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "AnOtherFile.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument hardwarePluginsPath in configuration file
/// \result         No Error - the hardware plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_Hardware_Plugins)
{
  //Creation of the config file
  write_settings( "hardwarePluginsPath", "try" );
  CreateDirectory ("try");

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"try");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
  RemoveDirectory("try");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument devicePluginsPath in configuration file
/// \result         No Error - the device plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_Device_Plugins)
{
  //Creation of the config file
  write_settings( "widgetsPath", "try" );
  CreateDirectory ("try");

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"try");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
  RemoveDirectory("try");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument disableXplHubStart in configuration file
/// \result         No Error - the Xpl is off
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_XPL_Option)
{
  //Creation of the config file
  write_settings( "disableXplHubStart", "false" );

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument webServerIp in configuration file
/// \result         No Error - the web Server IP address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_getWebServerIPAddress)
{
  //Creation of the config file
  write_settings( "webServerIp", "192.168.1.1" );

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.1");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument webServerIp in configuration file
/// \result         No Error - the web Server path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_webServerPath)
{
  //Creation of the config file
  write_settings( "webServerPath", "home" );
  CreateDirectory ("home");

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "home");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
  RemoveDirectory ("home");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument webServerIp in configuration file
/// \result         No Error - the log level is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_Log)
{
  //Creation of the config file
  write_settings( "logLevel", "warning" );

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the all arguments
/// \result         No Error - all options are changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_All_Options1)
{
  //Creation of the config file
  write_settings( "logLevel"            , "warning" );
  write_settings( "port"                , "8085" );
  write_settings( "databaseFile"        , "test.db3" );
  write_settings( "hardwarePluginsPath" , "try1" );
  write_settings( "widgetsPath"         , "try2" );
  write_settings( "disableXplHubStart"  , "false" );
  write_settings( "webServerIp"         , "192.168.1.2" );
  write_settings( "webServerPath"       , "try3" );

  CreateDirectory ("try1");
  CreateDirectory ("try2");
  CreateDirectory ("try3");

  startupOptions::CLoader StartupOptions (0, NULL);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "test.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"try1");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"try2");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.2");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "try3");

  //remove the configuration before tests
  RemoveFile ("yadoms.cfg");

  RemoveDirectory ("try1");
  RemoveDirectory ("try2");
  RemoveDirectory ("try3");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the all arguments with argc/argv
/// \result         No Error - all options are changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_All_Options2)
{
  char *argv[] = {"./MainTestStartOptionsLoader","--widgetsPath","try2","--port","8085","--databaseFile","test.db3","--hardwarePluginsPath","try1", "--disableXplHubStart","false","--webServerIp","192.168.1.3","--webServerPath","try3","--logLevel","warning"};

  CreateDirectory ("try1");
  CreateDirectory ("try2");
  CreateDirectory ("try3");

  startupOptions::CLoader StartupOptions (17, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "test.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"try1");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"try2");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.3");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "try3");

  RemoveDirectory ("try1");
  RemoveDirectory ("try2");
  RemoveDirectory ("try3");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the all arguments (low profile) by argc/argv
/// \result         No Error - all options are changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_All_Options3)
{
  char *argv[] = {"./MainTestStartOptionsLoader","-p","8085","-D","test.db3","-h","try1", "-x","false","-i","192.168.1.3","-l","warning", "-w", "try3"};

  CreateDirectory ("try1");
  CreateDirectory ("try2");
  CreateDirectory ("try3");

  startupOptions::CLoader StartupOptions (15, argv);

  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "test.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"try1");
  BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.3");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "try3");

  RemoveDirectory ("try1");
  RemoveDirectory ("try2");
  RemoveDirectory ("try3");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the parameter -p without value
/// \result         No Error - No Exception thrown
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Parameter_Missing_No_Exception)
{ 
  char *argv[] = {"./MainTestStartOptionsLoader","-p"};

  BOOST_REQUIRE_NO_THROW( startupOptions::CLoader StartupOptions (2, argv) );

}

//BOOST_AUTO_TEST_SUITE_END()
