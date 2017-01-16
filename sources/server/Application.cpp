#include "stdafx.h"
#include "Application.h"
#include "logging/LogConfiguration.h"
#include <shared/Log.h>

#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

#include "Supervisor.h"
#include "ErrorHandler.h"
#include "RunningInformation.h"
#include "PathProvider.h"
#include <shared/ServiceLocator.h>
#include <shared/process/ApplicationStopHandler.h>

//define the main entry point
POCO_SERVER_MAIN(CYadomsServer)


CYadomsServer::CYadomsServer()
   : m_helpRequested(false),
     m_startupOptions(boost::make_shared<startupOptions::CStartupOptions>(config()))
{
   //define unixstyle for command line parsing
   //so in Windows platform we use --option and -o for options (instead of /option)
   setUnixOptions(true);
}

CYadomsServer::~CYadomsServer()
{
}


void CYadomsServer::initialize(Application& self)
{
   loadConfiguration(); // load default configuration files, if present
   Poco::Util::ServerApplication::initialize(self);

   boost::filesystem::path workingDir(config().getString("application.path"));
   boost::filesystem::current_path(workingDir.parent_path());

   logging::CLogConfiguration::configure(m_startupOptions->getLogLevel());
}

void CYadomsServer::uninitialize()
{
   YADOMS_LOG(information) << "Yadoms is shutting down";
   ServerApplication::uninitialize();
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

int CYadomsServer::main(const ArgVec& /*args*/)
{
   enum
      {
         kApplicationFullyStopped = shared::event::kUserFirstId
      };

   if (!m_helpRequested)
   {
      auto executablePath = config().getString("application.path");
      m_runningInformation = boost::make_shared<CRunningInformation>(executablePath);
      CPathProvider pathProvider(m_startupOptions);


      YADOMS_LOG_CONFIGURE("Main");

      YADOMS_LOG(information) << "********************************************************************";
      YADOMS_LOG(information) << "Yadoms is starting";
      YADOMS_LOG(information) << "********************************************************************";
      if (m_startupOptions->getDeveloperMode())
      {
         YADOMS_LOG(information) << "** !! Developer Mode Enabled !! **";
         YADOMS_LOG(information) << "********************************************************************";
      }

      YADOMS_LOG(information) << "Startup options :";
      YADOMS_LOG(information) << "\tlog level = " << m_startupOptions->getLogLevel();
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
         YADOMS_LOG(information) << "\t\tPostgresql host = " << m_startupOptions->getDatabasePostgresqlHost() << ":" << m_startupOptions->getDatabasePostgresqlPort();
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
      shared::CServiceLocator::instance().push<startupOptions::IStartupOptions>(m_startupOptions);
      shared::CServiceLocator::instance().push<IRunningInformation>(m_runningInformation);

      //configure the Poco ErrorHandler
      CErrorHandler eh;
      auto pOldEH = Poco::ErrorHandler::set(&eh);

      //configure stop handler
      enum { kTerminationRequested = shared::event::kUserFirstId };
      auto stopHandler = boost::make_shared<shared::process::CApplicationStopHandler>(m_startupOptions->getIsRunningAsService());
      stopHandler->setApplicationStopHandler([&]() -> bool
         {
            // Ask for application stop and wait for application full stop
            m_stopRequestEventHandler.postEvent(kTerminationRequested);
            return m_stoppedEventHandler.waitForEvents(boost::posix_time::seconds(30)) == kApplicationFullyStopped;
         });

      //create supervisor
      CSupervisor supervisor(pathProvider);
      Poco::Thread supervisorThread("Supervisor");
      supervisorThread.start(supervisor);

      //
      // Yadoms is running...
      //

      // Wait for stop
      YADOMS_LOG(debug) << "Yadoms is running...";
      while (m_stopRequestEventHandler.waitForEvents() != kTerminationRequested)
      {
         YADOMS_LOG(warning) << "Wrong application stop event received : " << m_stopRequestEventHandler.getEventId();
      }

      YADOMS_LOG(debug) << "Receive termination request : ask supervisor to stop...";
      supervisor.requestToStop();
      supervisorThread.join();
      YADOMS_LOG(debug) << "Supervisor stopped";

      //restore Poco ErrorHandler
      Poco::ErrorHandler::set(pOldEH);
   }

   YADOMS_LOG(debug) << "Yadoms is stopped";
   m_stoppedEventHandler.postEvent(kApplicationFullyStopped);
   return EXIT_OK;
}

