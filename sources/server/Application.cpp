#include "stdafx.h"
#include "Application.h"
#include "logging/LogConfiguration.h"
#include <shared/Log.h>

#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPStreamFactory.h>

#include "Supervisor.h"
#include "ErrorHandler.h"
#include "RunningInformation.h"
#include "PathProvider.h"
#include <shared/ServiceLocator.h>
#include <shared/process/ApplicationStopHandler.h>
#include <openssl/opensslv.h>
#include <Poco/Environment.h>
#include <Poco/Format.h>
#include <google/protobuf/stubs/common.h>
#include "shared/http/HttpMethods.h"

//define the main entry point
POCO_SERVER_MAIN(CYadomsServer)

static const shared::versioning::CSemVer YadomsVersion(YADOMS_VERSION);


CYadomsServer::CYadomsServer()
   : m_helpRequested(false),
     m_startupOptions(boost::make_shared<startupOptions::CStartupOptions>(config()))
{
   //define unixstyle for command line parsing
   //so in Windows platform we use --option and -o for options (instead of /option)
   setUnixOptions(true);
}


void CYadomsServer::initialize(Application& self)
{
   //[START] INITIALIZE POCO; DO NOT EDIT THIS CODE
   loadConfiguration(); // load default configuration files, if present
   ServerApplication::initialize(self);
   //[END] INITIALIZE POCO; DO NOT EDIT THIS CODE

   //Do application initialization from here...

   //first of all, change the working dir
   //in daemon mode, poco change dir to '/', (path provider create logs, data, dirs => so fails if working dir is '/')
   const boost::filesystem::path workingDir(config().getString("application.path"));
   current_path(workingDir.parent_path());

   m_pathProvider = boost::make_shared<CPathProvider>(m_startupOptions);
   logging::CLogConfiguration::configure(m_startupOptions->getLogLevel(), m_pathProvider->logsPath());

   //define proxy settings as earlier possible
   setupProxy();
   Poco::Net::HTTPStreamFactory::registerFactory();
}

void CYadomsServer::setupProxy() const
{
   if (m_startupOptions->getProxyHost().isNull())
      return;

   const auto host = m_startupOptions->getProxyHost().value();
   const auto port = m_startupOptions->getProxyPort().isNull()
                        ? shared::http::CHttpMethods::kUseProxyDefaultPort
                        : m_startupOptions->getProxyPort().value();
   const auto username = m_startupOptions->getProxyUsername().isNull()
                            ? std::string()
                            : m_startupOptions->getProxyUsername().value();
   const auto password = m_startupOptions->getProxyPassword().isNull()
                            ? std::string()
                            : m_startupOptions->getProxyPassword().value();
   const auto bypassRegex = m_startupOptions->getProxyBypass().isNull()
                               ? std::string()
                               : m_startupOptions->getProxyBypass().value();
   shared::http::CHttpMethods::setGlobalProxyConfig(host,
                                                    port,
                                                    username,
                                                    password,
                                                    bypassRegex);
}

void CYadomsServer::uninitialize()
{
   ServerApplication::uninitialize();
   if (!m_helpRequested)
   {
      std::cout << "Yadoms is stopped" << std::endl;
   }
}

void CYadomsServer::defineOptions(Poco::Util::OptionSet& options)
{
   //configure Poco::Application options
   ServerApplication::defineOptions(options);

   //manage only help option
   options.addOption(
      Poco::Util::Option("help", "h", "display help information on command line arguments")
      .required(false)
      .repeatable(false)
      .callback(Poco::Util::OptionCallback<CYadomsServer>(this, &CYadomsServer::handleHelp)));

   options.addOption(
      Poco::Util::Option("version", "v", "display version")
      .required(false)
      .repeatable(false)
      .callback(Poco::Util::OptionCallback<CYadomsServer>(this, &CYadomsServer::handleVersion)));

   //configure startup yadoms options
   m_startupOptions->defineOptions(options);
}

void CYadomsServer::handleHelp(const std::string& /*name*/, const std::string& /*value*/)
{
   m_helpRequested = true;
   displayHelp();
   stopOptionsProcessing();
}

void CYadomsServer::displayHelp() const
{
   Poco::Util::HelpFormatter helpFormatter(options());
   helpFormatter.setUnixStyle(true);
   helpFormatter.setCommand(commandName());
   helpFormatter.setUsage("OPTIONS");
   helpFormatter.setHeader("Yadoms command line usage");
   helpFormatter.format(std::cout);
}

void CYadomsServer::handleVersion(const std::string& name, const std::string& value)
{
   m_helpRequested = true;
   displayVersion();
   stopOptionsProcessing();
}

void CYadomsServer::displayVersion() const
{
   //output string on standard output
   //because log is not defined (and will not => after calling "yadoms --version", it show version and exit)
   std::cout << "Yadoms version " << YadomsVersion << std::endl;

   std::cout << "Libraries: " << std::endl;
   std::cout << "    boost: " << (BOOST_VERSION / 100000) << "." << ((BOOST_VERSION / 100) % 1000) << "." << (
      BOOST_VERSION % 100) << std::endl;
   std::cout << "    " << OPENSSL_VERSION_TEXT << std::endl;

   const std::string version = Poco::format("%d.%d.%d",
                                            static_cast<int>(Poco::Environment::libraryVersion() >> 24),
                                            static_cast<int>((Poco::Environment::libraryVersion() >> 16) & 0xFF),
                                            static_cast<int>((Poco::Environment::libraryVersion() >> 8) & 0xFF));
   std::cout << "    poco: " << version << std::endl;
   std::cout << "    protobuf: " << GOOGLE_PROTOBUF_VERSION_SUFFIX << " " << google::protobuf::internal::
      VersionString(GOOGLE_PROTOBUF_VERSION) << std::endl;
}


int CYadomsServer::main(const ArgVec&)
{
   try
   {
      enum
      {
         kApplicationFullyStopped = shared::event::kUserFirstId
      };
      auto stoppedEventHandler = boost::make_shared<shared::event::CEventHandler>();

      if (!m_helpRequested)
      {
         auto executablePath = config().getString("application.path");
         m_runningInformation = boost::make_shared<CRunningInformation>(executablePath,
                                                                        YadomsVersion);


         YADOMS_LOG_CONFIGURE("Main");

         YADOMS_LOG(information) << "\n********************************************************************";
         YADOMS_LOG(information) << "Yadoms " << YadomsVersion << " is starting";
         YADOMS_LOG(information) << "********************************************************************";
         if (m_startupOptions->getDeveloperMode())
         {
            YADOMS_LOG(information) << "** !! Developer Mode Enabled !! **";
            YADOMS_LOG(information) << "********************************************************************";
         }

         YADOMS_LOG(information) << "Startup options :";
         YADOMS_LOG(information) << "\tLog level = " << m_startupOptions->getLogLevel();
         YADOMS_LOG(information) << "\tLog path = " << m_startupOptions->getLogPath();
         YADOMS_LOG(information) << "\tWeb server port number = " << m_startupOptions->getWebServerPortNumber();
         YADOMS_LOG(information) << "\tSSL activated = " << m_startupOptions->getIsWebServerUseSSL();
         YADOMS_LOG(information) << "\tSSL Web server port number = " << m_startupOptions->getSSLWebServerPortNumber();
         YADOMS_LOG(information) << "\tWeb server ip = " << m_startupOptions->getWebServerIPAddress();
         YADOMS_LOG(information) << "\tWeb server path = " << m_startupOptions->getWebServerInitialPath();
         YADOMS_LOG(information) << "\tDatabase engine = " << m_startupOptions->getDatabaseEngine();

         switch (m_startupOptions->getDatabaseEngine())
         {
         case startupOptions::EDatabaseEngine::kSqliteValue:
            YADOMS_LOG(information) << "\t\tSqlite Database path = " << m_startupOptions->getDatabaseSqliteFile();
            break;
         case startupOptions::EDatabaseEngine::kPostgresqlValue:
            YADOMS_LOG(information) << "\t\tPostgresql host = " << m_startupOptions->getDatabasePostgresqlHost() << ":"
               << m_startupOptions->getDatabasePostgresqlPort();
            YADOMS_LOG(information) << "\t\tPostgresql database = " << m_startupOptions->getDatabasePostgresqlDbName();
            YADOMS_LOG(information) << "\t\tPostgresql login = " << m_startupOptions->getDatabasePostgresqlLogin();
            break;
         default:
            YADOMS_LOG(error) << "\t\tUnsupported database engine" << m_startupOptions->getDatabaseEngine();
            break;
         }


         if (m_startupOptions->getNoPasswordFlag())
            YADOMS_LOG(information) << "\tnoPassword = true";
         YADOMS_LOG(information) << "********************************************************************";

         //register Services in serviceLocator
         shared::CServiceLocator::instance().push<const startupOptions::IStartupOptions>(m_startupOptions);
         shared::CServiceLocator::instance().push<IRunningInformation>(m_runningInformation);

         //configure the Poco ErrorHandler
         CErrorHandler eh;
         const auto oldErrorHandler = Poco::ErrorHandler::set(&eh);

         //configure stop handler
         enum
         {
            kTerminationRequested = shared::event::kUserFirstId
         };
         auto stopRequestEventHandler = boost::make_shared<shared::event::CEventHandler>();
         auto stopHandler = boost::make_shared<shared::process::CApplicationStopHandler>(
            m_startupOptions->getIsRunningAsService());
         stopHandler->setApplicationStopHandler([stopRequestEventHandler, stoppedEventHandler]() -> bool
         {
            // Ask for application stop and wait for application full stop
            YADOMS_LOG(debug) << "Receive termination request";
            stopRequestEventHandler->postEvent(kTerminationRequested);
            const auto stopSuccess = stoppedEventHandler->waitForEvents(boost::posix_time::seconds(30)) ==
               kApplicationFullyStopped;
            if (!stopSuccess)
               YADOMS_LOG(error) << "Fail to wait the app end event";
            return stopSuccess;
         });

         //create supervisor
         CSupervisor supervisor(m_pathProvider,
                                YadomsVersion);
         Poco::Thread supervisorThread("Supervisor");
         supervisorThread.start(supervisor);

         //
         // Yadoms is running...
         //

         // Wait for stop
         YADOMS_LOG(debug) << "Yadoms is running...";
         while (stopRequestEventHandler->waitForEvents() != kTerminationRequested)
         {
            YADOMS_LOG(warning) << "Wrong application stop event received : " << stopRequestEventHandler->getEventId();
         }

         YADOMS_LOG(information) << "Receive termination request : ask supervisor to stop...";
         supervisor.requestToStop();
         supervisorThread.join();
         YADOMS_LOG(debug) << "Supervisor stopped";

         //restore Poco ErrorHandler
         Poco::ErrorHandler::set(oldErrorHandler);
      }

      if (!m_helpRequested)
      {
         YADOMS_LOG(information) << "Yadoms is stopped";
      }
      stoppedEventHandler->postEvent(kApplicationFullyStopped);
      return EXIT_OK;
   }
   catch (...)
   {
      return EXIT_FAILURE;
   }
}
