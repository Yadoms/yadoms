// \brief #define BOOST_AUTO_TEST_MAIN This definition is now configured into the CmakeFile
#include "stdafx.h"

/*
\file main.cpp

\brief Can use "brief" tag to explicitly generate comments for file documentation.

*/

#include <iostream>
#include <fstream>
#include "../../../sources/server/StartupOptionsLoader.h"
#include "../../../sources/server/pluginSystem/HardwarePluginManager.h"
#include "../../../sources/server/database/IDataProvider.h"
#include "../../../sources/server/database/sqlite/SQLiteDataProvider.h"
//#include "../../sources/server/ApplicationStopHandler.h"

#define BOOST_TEST_MODULE TestStartupOptionsLoader

#include <boost/test/unit_test.hpp>

/* TODO : A voir pour sortie directe en xml
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

/*bool validate(CStartupOptionsLoaderException str)
{
 return str.isError();
}*/

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

void write_settings ( std::string name, std::string value )
{
	std::ofstream settings_file;

	settings_file.open( "yadoms.cfg", std::fstream::out | std::fstream::app );

	settings_file << name.c_str() << " = " << value.c_str() << "\n";
	settings_file.close();
}

void CreateDirectory ( std::string name )
{
	//const char dir_path[] = "c:\\temp\\cplusplus";

	boost::filesystem::path dir(name.c_str());
	if(boost::filesystem::create_directory(dir)) {
	//	std::cout << "Success" << "\n";
	}
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

//BOOST_AUTO_TEST_SUITE(Initialisation)

//--------------------------------------------------------------
/// \brief	    Test CStartupOptionsLoader with no argument
/// \result         No Error
//--------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Initialisation_Test)
{
	//remove the configuration before tests
	RemoveFile ("yadoms.cfg");

  CStartupOptionsLoader m_startupOptions(0, NULL);
  
  boost::shared_ptr<IDataProvider> pDataProvider (new CSQLiteDataProvider(m_startupOptions.getDatabaseFile()));

  std::string value ("");
  boost::shared_ptr<IHardwareRequester> database;
  boost::shared_ptr<CHardwarePluginManager> hardwarePluginManager = CHardwarePluginManager::newHardwarePluginManager(
         m_startupOptions.getHarwarePluginsPath(), pDataProvider->getHardwareRequester());
/*
  BOOST_CHECK_EQUAL(StartupOptions.getLogLevel(), boost::log::trivial::info);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerPortNumber(), 8080);
  BOOST_CHECK_EQUAL(StartupOptions.getDatabaseFile(), "yadoms.db3");
  BOOST_CHECK_EQUAL(StartupOptions.getHarwarePluginsPath(),"plugins/hardware");
  BOOST_CHECK_EQUAL(StartupOptions.getDevicePluginsPath(),"plugins/device");
  BOOST_CHECK_EQUAL(StartupOptions.getStartXplHubFlag(),true);
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerIPAddress(), "0.0.0.0");
  BOOST_CHECK_EQUAL(StartupOptions.getWebServerInitialPath(), "www");
*/
}

//BOOST_AUTO_TEST_SUITE_END()
