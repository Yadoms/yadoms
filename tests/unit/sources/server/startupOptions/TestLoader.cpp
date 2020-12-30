#include "stdafx.h"


#include <boost/test/unit_test.hpp>

// Includes needed to compile the test
#include "../../testCommon/fileSystem.h"



class CTestPath
{
public:
   explicit CTestPath(const std::string& testDirectory)
      : m_testDirectory(testDirectory)
   {
      // Ensure TestDirectory is empty
      try
      {
         testCommon::filesystem::removeDirectory(m_testDirectory);
      }
      catch (boost::filesystem::filesystem_error&)
      {
         // Under Windows, directory can be locked for a little duration after directory creation
         // So wait a bit and re-try
         boost::this_thread::sleep(boost::posix_time::millisec(500));
         testCommon::filesystem::removeDirectory(m_testDirectory);
      }

      // Create directory
      testCommon::filesystem::createDirectory(m_testDirectory);
   }

   virtual ~CTestPath()
   {
      // Clean-up TestDirectory
      try
      {
         testCommon::filesystem::removeDirectory(m_testDirectory);
      }
      catch(boost::filesystem::filesystem_error&)
      {
         // Under Windows, directory can be locked for a little duration after directory creation
         // So wait a bit and re-try
         boost::this_thread::sleep(boost::posix_time::millisec(500));
         testCommon::filesystem::removeDirectory(m_testDirectory);
      }
   }

private:
   const std::string& m_testDirectory;
};

class CTestConfigFile
{
public:
   CTestConfigFile()
      : m_configFile("yadoms.cfg")
   {
      // Ensure file doesn't exist
      testCommon::filesystem::removeFile(m_configFile, false);
   }

   virtual ~CTestConfigFile()
   {
      // Clean-up
      testCommon::filesystem::removeFile(m_configFile, false);
   }

   void writeSettings(std::string setting, std::string value) const
   {
      std::ofstream file(m_configFile.c_str(), std::ios_base::out | std::ios_base::app);
      file << setting << " = " << value << "\n";
      file.close();
   }

private:
   const std::string m_configFile;
};

static const std::string testNewWebServerPath("newNewWebServerPath");
static const std::string testFalsePath("FalsePath");


// Includes needed to compile tested classes
#include "../../../../sources/server/startupOptions/StartupOptions.h"

#include <Poco/Util/Application.h>
#include <Poco/Util/OptionProcessor.h>
#include <Poco/Util/Validator.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/SystemConfiguration.h>
#include <Poco/Util/MapConfiguration.h>
#include <Poco/Util/OptionException.h>

BOOST_AUTO_TEST_SUITE(TestLoader)


   class CStartupOptionMockup
   {
   public:
      class CMyConf : public Poco::Util::MapConfiguration
      {
      public:
         CMyConf()
         {
         }

         virtual ~CMyConf()
         {
         }
      };

      Poco::Util::OptionSet os;

      CStartupOptionMockup(int argc, char* argv[], bool unixStyle)
         :m_config(new CMyConf), m_options(*(m_config.get()))
      {
         
         m_options.defineOptions(os);
         Poco::Util::OptionProcessor processor(os);
         processor.setUnixStyle(unixStyle);
         for (auto i = 0; i < argc; ++i)
         {
            std::string name;
            std::string value;
            if (processor.process(argv[i], name, value))
            {
               if (!name.empty()) // "--" option to end options processing or deferred argument
               {
                  handleOption(os, name, value);
               }
            }
         }
         processor.checkRequired();
         
      }

      startupOptions::CStartupOptions& options()
      {
         return m_options;
      }

      Poco::AutoPtr<Poco::Util::AbstractConfiguration>& config()
      {
         return m_config;
      }

   private:
      void handleOption(Poco::Util::OptionSet& os, const std::string& name, const std::string& value)
      {
         const auto& option = os.getOption(name);
         if (option.validator())
         {
            option.validator()->validate(option, value);
         }
         if (!option.binding().empty())
         {
            m_config->setString(std::string(option.binding()), std::string(value));
         }
         if (option.callback())
         {
            option.callback()->invoke(name, value);
         }
      }


      Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_config;
      startupOptions::CStartupOptions m_options;
   };


   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with no argument
   /// \result         No Error
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Initialisation_Test)
   {
      CStartupOptionMockup loader(0, nullptr, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the help requested "--help"
   /// \result         CLoaderException exception expected
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(helpRequest)
   {
      char* argv[] = {"./TestLoader", "-help"};
      BOOST_CHECK_THROW(CStartupOptionMockup loader(2, argv, true), std::exception) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the help Mockup (short form) "-h"
   /// \result         CLoaderException exception expected
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(helpRequestShort)
   {
      char* argv[] = {"./TestLoader", "-h"};
      BOOST_CHECK_THROW(CStartupOptionMockup loader(2, argv, true), std::exception) ;
   }


   BOOST_AUTO_TEST_CASE(Different_Port_p_Initialisation)
   {
      char* argv[] = {"./TestLoader", "--port:2000"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(2000)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the change of the port with --port
   /// \result         No Error - port number change
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Port_port_Initialisation)
   {
      char* argv[] = {"./TestLoader", "--port", "2000"};

      CStartupOptionMockup loader(3, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(2000)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with an argument error : --por
   /// \result         No Error, syntax accepted since no ambiguity with other option - port number change
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Port_por_Initialisation)
   {
      char* argv[] = {"./TestLoader", "--por", "2000"};

      CStartupOptionMockup loader(3, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(2000)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with an argument error : --webServer
   /// \result         Error : Raise an Exception (ambiguous webServerIp or webServerPath ?)
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Port_Initialisation_Error1)
   {
      char* argv[] = {"./TestLoader", "--webServer", "192.168.1.1"};

      BOOST_CHECK_THROW(CStartupOptionMockup loader(3, argv, true), Poco::Exception) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with an argument error : -port
   /// \result         Error : Raise an Exception
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Port_Initialisation_Error2)
   {
      char* argv[] = {"./TestLoader", "-port", "2000"};

      //Test the exception, and if this one is the correct one !
      BOOST_CHECK_THROW(CStartupOptionMockup loader(3, argv, true), Poco::Exception) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument --databaseFile
   /// \result         No Error - database name change
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Database_databaseFile_Initialisation)
   {
      char* argv[] = {"./TestLoader", "--databaseSqliteFile:toto.db3"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "toto.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -d
   /// \result         No Error - database name change
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Database_d_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-Dtoto.db3"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "toto.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to trace
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_trace_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-ltrace"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "trace") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to debug
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_debug_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-ldebug"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "debug") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to info
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_info_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-linformation"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to warning
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_warning_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-lwarning"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "warning") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to error
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_error_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-lerror"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "error") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to fatal
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_fatal_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-lfatal"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "fatal") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }


   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to notice
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_notice_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-lnotice"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "notice") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -l
   /// \result         No Error - logs change to notice
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_Log_l_critical_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-lcritical"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "critical") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with all logger levels
   /// \result         No error, no exception
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(All_Loggerlevels)
   {
      static const std::set<std::string> levels = boost::assign::list_of("none")("fatal")("critical")("error")("warning")("notice")("information")("debug")("trace");
      for (std::set<std::string>::const_iterator it = levels.begin(); it != levels.end(); ++it)
      {
         std::string opt("-l");
         opt += *it;
         char* argv[] = {"./TestLoader", const_cast<char*>(opt.c_str())};
         CStartupOptionMockup loader(2, argv, true);
         BOOST_CHECK_EQUAL(loader.options().getLogLevel(), *it) ;
      }
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with a faulty log argument -l
   /// \result         Raise an exception
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Unknow_Log_l_Error1)
   {
      char* argv[] = {"./TestLoader", "-ltoto"};
      BOOST_CHECK_THROW(CStartupOptionMockup loader(2, argv, true), Poco::Util::InvalidArgumentException) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with a faulty argument entry
   /// \result         Exception is raised
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Unknow_option_NoError)
   {
      char* argv[] = {"./TestLoader", "-binfo"};
      BOOST_CHECK_THROW(CStartupOptionMockup loader(2, argv, true), Poco::Util::UnknownOptionException) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -i
   /// \result         No Error - the adresse IP is changed
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_IP_i_Initialisation)
   {
      char* argv[] = {"./TestLoader", "-i192.168.1.1"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "192.168.1.1") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -i
   /// \result         No Error - the adresse IP is changed
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Initialisation)
   {
      char* argv[] = {"./TestLoader", "--webServerIp:192.168.1.1"};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "192.168.1.1") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), "www") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the wrong argument -webServe
   /// \result         Raise a Exception
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Error1)
   {
      char* argv[] = {"./TestLoader", "--webServe:192.168.1.1"};

      BOOST_CHECK_THROW(CStartupOptionMockup loader(2, argv, true), Poco::Exception) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with a malform IP address (not complete)
   /// \result         Raise a Exception
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_IP_webServerIp_Error2)
   {
      char* argv[] = {"./TestLoader", "-i:192.168.1."};

      BOOST_CHECK_THROW(CStartupOptionMockup loader(2, argv, true), Poco::Exception) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument -w
   /// \result         No Error - the website address is changed
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_WebServer_w_Initialisation)
   {
      CTestPath webServerPath(testNewWebServerPath);
      std::string arg = "-w";
      arg += testNewWebServerPath;

      char* argv[] = {"./TestLoader", const_cast<char*>(arg.c_str())};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), testNewWebServerPath) ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument --webServerPath
   /// \result         No Error - the website address is changed
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_Initialisation)
   {
      CTestPath webServerPath(testNewWebServerPath);
      std::string arg = "--webServerPath:";
      arg += testNewWebServerPath;
      char* argv[] = {"./TestLoader", const_cast<char*>(arg.c_str())};

      CStartupOptionMockup loader(2, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "information") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8080)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "0.0.0.0") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), testNewWebServerPath) ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "yadoms.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the argument --webServerPath with a wrong Path
   /// \result         Raise a Exception
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Different_WebServer_webServerPath_WrongPath)
   {
      CTestPath webServerPath(testNewWebServerPath);

      std::string arg = "--webServerPath:";
      arg += testFalsePath;

      char* argv[] = {"./TestLoader", const_cast<char*>(arg.c_str())};

      BOOST_CHECK_THROW(CStartupOptionMockup loader(2, argv, true), Poco::Exception) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the all arguments with argc/argv
   /// \result         No Error - all options are changed
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(All_Options1)
   {
      CTestPath webServerPath(testNewWebServerPath);

      char* argv[] =
         {
            "./TestLoader",
            "--port", "8085",
            "--databaseSqliteFile", "test.db3",
            "--webServerIp", "192.168.1.3",
            "--webServerPath", const_cast<char*>(testNewWebServerPath.c_str()),
            "--logLevel", "warning"
         };

      CStartupOptionMockup loader(11, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "warning") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8085)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "192.168.1.3") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), testNewWebServerPath) ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "test.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the all arguments (low profile) by argc/argv
   /// \result         No Error - all options are changed
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(All_Options2)
   {
      CTestPath webServerPath(testNewWebServerPath);

      std::string arg = "-w:";
      arg += testNewWebServerPath;

      char* argv[] =
         {
            "./TestLoader",
            "-p8085",
            "-Dtest.db3",
            "-i192.168.1.3",
            "-lwarning",
            const_cast<char*>(arg.c_str())
         };

      CStartupOptionMockup loader(6, argv, true);

      BOOST_CHECK_EQUAL(loader.options().getLogLevel(), "warning") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerPortNumber(), static_cast<unsigned int>(8085)) ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerIPAddress(), "192.168.1.3") ;
      BOOST_CHECK_EQUAL(loader.options().getWebServerInitialPath(), testNewWebServerPath) ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseSqliteFile(), "test.db3") ;
      BOOST_CHECK_EQUAL(loader.options().getPluginsPath(), "plugins") ;
      BOOST_CHECK_EQUAL(loader.options().getScriptInterpretersPath(), "scriptInterpreters") ;
      BOOST_CHECK_EQUAL(loader.options().getNoPasswordFlag(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getIsRunningAsService(), false) ;
      BOOST_CHECK_EQUAL(loader.options().getUpdateSiteUri(), "http://www.yadoms.com/downloads/update/") ;
      BOOST_CHECK_EQUAL(loader.options().getDatabaseAcquisitionLifetime(), 30) ;
      BOOST_CHECK_EQUAL(loader.options().getDeveloperMode(), false) ;
   }

   //--------------------------------------------------------------
   /// \brief	    Test CStartupOptionMockup with the parameter -p without value
   /// \result         No Error - No Exception thrown
   //--------------------------------------------------------------

   BOOST_AUTO_TEST_CASE(Parameter_Missing_No_Exception)
   {
      char* argv[] = {"./TestLoader", "-p"};

      BOOST_CHECK_THROW(CStartupOptionMockup app(2, argv, true), Poco::Exception) ;
   }

   BOOST_AUTO_TEST_SUITE_END()

