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

bool validate(startupOptions::CLoaderException exception)
{
   return exception.isError();
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

class CTestPath
{
public:
   CTestPath(const std::string& testDirectory)
      :m_testDirectory(testDirectory)
   {
      // Ensure TestDirectory is empty
      RemoveDirectory(m_testDirectory);
      CreateDirectory(m_testDirectory);
   }
   virtual ~CTestPath()
   {
      // Clean-up TestDirectory
      RemoveDirectory(m_testDirectory);
   }
private:
   const std::string& m_testDirectory;
};

class CTestConfigFile
{
public:
   CTestConfigFile()
      :m_configFile("yadoms.cfg")
   {
      // Ensure file doesn't exist
      RemoveFile(m_configFile);
   }
   virtual ~CTestConfigFile()
   {
      // Clean-up
      RemoveFile(m_configFile);
   }
   void writeSettings (std::string setting, std::string value)
   {
      std::ofstream file;
      file.open(m_configFile, std::fstream::out | std::fstream::app );
      file << setting.c_str() << " = " << value.c_str() << "\n";
      file.close();
   }
private:
   const std::string m_configFile;
};

static const std::string testNewPluginPath = "newPluginPath";
static const std::string testNewWidgetsPath = "newWidgetsPath";
static const std::string testNewWebServerPath = "newNewWebServerPath";


//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with no argument
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Initialisation_Test)
{
   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-p","2000"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)2000);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","--port","2000"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)2000);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","--por","2000"};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with an argument error : -port
/// \result         Error : Raise an Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Port_Initialisation_Error2)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","-port","2000"};

   //Test the exception, and if this one is the correct one !
   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument --databaseFile
/// \result         No Error - database name change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Initialisation)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","--databaseFile","toto.db3"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","--database","toto.db3"};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -d
/// \result         No Error - database name change
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_Database_d_Initialisation)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","-D","toto.db3"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "toto.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-l","trace"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::trace);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-l","debug"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::debug);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-l","info"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-l","warning"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-l","error"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::error);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-l","fatal"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::fatal);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","-l","toto"};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with a faulty argument entry
/// \result         Raise an exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Unknow_option_Error2)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","-a","info"};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -i
/// \result         No Error - the adresse IP is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_i_Initialisation)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","-i","192.168.1.1"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","--webServerIp","192.168.1.1"};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","--webServe","192.168.1.1"};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with a malform IP address (not complete)
/// \result         Raise a Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Error2)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","-i","192.168.1."};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -w
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_w_Initialisation)
{
   CTestPath webServerPath(testNewWebServerPath);
   const char *argv[] = {"./MainTestStartOptionsLoader","-w",testNewWebServerPath.c_str()};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), testNewWebServerPath);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument --webServerPath
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_Initialisation)
{
   CTestPath webServerPath(testNewWebServerPath);
   const char *argv[] = {"./MainTestStartOptionsLoader","--webServerPath",testNewWebServerPath.c_str()};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), testNewWebServerPath);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument --webServerPath
/// \result         No Error - the website address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_WrongPath)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","--webServerPath","wrongPath"};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (3, argv), startupOptions::CLoaderException, validate);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -P
/// \result         No Error - the plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_PluginsPath_h_Initialisation)
{
   CTestPath pluginPath(testNewPluginPath);
   const char *argv[] = {"./MainTestStartOptionsLoader","-P",testNewPluginPath.c_str()};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),testNewPluginPath);
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument --pluginsPath
/// \result         No Error - the plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_PluginsPath_PluginsPath_Initialisation)
{
   CTestPath pluginPath(testNewPluginPath);
   const char *argv[] = {"./MainTestStartOptionsLoader","--pluginsPath",testNewPluginPath.c_str()};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),testNewPluginPath);
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument --widgetsPath
/// \result         No Error - the widgets path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Different_WidgetPath_WidgetPath_Initialisation)
{
   CTestPath widgetsPath(testNewWidgetsPath);
   const char *argv[] = {"./MainTestStartOptionsLoader","--widgetsPath",testNewWidgetsPath.c_str()};

   startupOptions::CLoader StartupOptions (3, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),testNewWidgetsPath);
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//TODO : Tester pluginsPath et widgetsPath avec un faux chemin

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument -x
/// \result         No Error - the XPL is disactivated
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Disable_XPL_x_Initialisation)
{
   const char *argv[] = {"./MainTestStartOptionsLoader","-x"};

   startupOptions::CLoader StartupOptions (2, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   const char *argv[] = {"./MainTestStartOptionsLoader","--disableXplHubStart"};

   startupOptions::CLoader StartupOptions (2, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
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
   CTestConfigFile config;
   config.writeSettings("port", "8085");

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument port in configuration file
/// \result         No Error - the port number is not changed - No Exception
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_Port_Number_Error1)
{
   CTestConfigFile config;
   config.writeSettings("portable", "8085");

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");

   // There should no have exception !
   BOOST_REQUIRE_NO_THROW( startupOptions::CLoader StartupOptions (0, NULL) );
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument databaseFile in configuration file
/// \result         No Error - the database file name is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_database_file)
{
   CTestConfigFile config;
   config.writeSettings("databaseFile", "AnOtherFile.db3");

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "AnOtherFile.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument PluginsPath in configuration file
/// \result         No Error - the plugins path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_PluginsPath)
{
   CTestConfigFile config;
   config.writeSettings("pluginsPath", testNewPluginPath);
   CTestPath pluginPath(testNewPluginPath);

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),testNewPluginPath);
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument widgetsPath in configuration file
/// \result         No Error - the widgets path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_WidgetsPath)
{
   CTestPath widgetsPath(testNewWidgetsPath);
   CTestConfigFile config;
   config.writeSettings("widgetsPath", testNewWidgetsPath);

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),testNewWidgetsPath);
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument disableXplHubStart in configuration file
/// \result         No Error - the Xpl is off
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_XPL_Option)
{
   CTestConfigFile config;
   config.writeSettings("disableXplHubStart", "false");

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument webServerIp in configuration file
/// \result         No Error - the web Server IP address is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_getWebServerIPAddress)
{
   CTestConfigFile config;
   config.writeSettings("webServerIp", "192.168.1.1");

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.1");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument webServerIp in configuration file
/// \result         No Error - the web Server path is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_webServerPath)
{
   CTestPath webServerPath(testNewWebServerPath);
   CTestConfigFile config;
   config.writeSettings("webServerPath", testNewWebServerPath);

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), testNewWebServerPath);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the argument webServerIp in configuration file
/// \result         No Error - the log level is changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_Log)
{
   CTestConfigFile config;
   config.writeSettings("logLevel", "warning");

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8080);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),"plugins");
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),"widgets");
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the all arguments
/// \result         No Error - all options are changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Config_File_All_Options1)
{
   CTestPath pluginsPath(testNewPluginPath);
   CTestPath widgetsPath(testNewWidgetsPath);
   CTestPath webServerPath(testNewWebServerPath);

   CTestConfigFile config;
   config.writeSettings("logLevel"            , "warning");
   config.writeSettings("port"                , "8085");
   config.writeSettings("databaseFile"        , "test.db3");
   config.writeSettings("pluginsPath"         , testNewPluginPath);
   config.writeSettings("widgetsPath"         , testNewWidgetsPath);
   config.writeSettings("disableXplHubStart"  , "false");
   config.writeSettings("webServerIp"         , "192.168.1.2");
   config.writeSettings("webServerPath"       , testNewWebServerPath);

   startupOptions::CLoader StartupOptions (0, NULL);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "test.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),testNewPluginPath);
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),testNewWidgetsPath);
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.2");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), testNewWebServerPath);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the all arguments with argc/argv
/// \result         No Error - all options are changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(All_Options1)
{
   CTestPath pluginsPath(testNewPluginPath);
   CTestPath widgetsPath(testNewWidgetsPath);
   CTestPath webServerPath(testNewWebServerPath);

   const char *argv[] =
   {
      "./MainTestStartOptionsLoader",
      "--widgetsPath",testNewWidgetsPath.c_str(),
      "--port","8085",
      "--databaseFile","test.db3",
      "--pluginsPath",testNewPluginPath.c_str(),
      "--disableXplHubStart","false",
      "--webServerIp","192.168.1.3",
      "--webServerPath",testNewWebServerPath.c_str(),
      "--logLevel","warning"
   };

   startupOptions::CLoader StartupOptions (17, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "test.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),testNewPluginPath);
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),testNewWidgetsPath);
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.3");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), testNewWebServerPath);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the all arguments (low profile) by argc/argv
/// \result         No Error - all options are changed
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(All_Options2)
{
   CTestPath pluginsPath(testNewPluginPath);
   CTestPath widgetsPath(testNewWidgetsPath);
   CTestPath webServerPath(testNewWebServerPath);

   const char *argv[] =
   {
      "./MainTestStartOptionsLoader",
      "-p","8085",
      "-D","test.db3",
      "-P",testNewPluginPath.c_str(),
      "-W",testNewWidgetsPath.c_str(),
      "-x","false",
      "-i","192.168.1.3",
      "-l","warning",
      "-w",testNewWebServerPath.c_str()
   };

   startupOptions::CLoader StartupOptions (17, argv);

   BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::warning);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), (unsigned int)8085);
   BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "test.db3");
   BOOST_CHECK_EQUAL(StartupOptions.getPluginsPath(),testNewPluginPath);
   BOOST_CHECK_EQUAL(StartupOptions.getWidgetsPath(),testNewWidgetsPath);
   BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),false);
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "192.168.1.3");
   BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(),testNewWebServerPath);
}

//--------------------------------------------------------------
/// \brief	    Test startupOptions::CLoader with the parameter -p without value
/// \result         No Error - No Exception thrown
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Parameter_Missing_No_Exception)
{ 
   const char *argv[] = {"./MainTestStartOptionsLoader","-p"};

   BOOST_REQUIRE_EXCEPTION(startupOptions::CLoader StartupOptions (2, argv), startupOptions::CLoaderException, validate);
}
