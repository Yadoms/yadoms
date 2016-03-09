#include "stdafx.h"
#include "Application.h"
#include "logConfiguration/LogConfiguration.h"
#include <shared/Log.h>

#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

#include "Supervisor.h"
#include "ErrorHandler.h"
#include "ApplicationStopHandler.h"
#include "RunningInformation.h"
#include <shared/currentTime/Provider.h>
#include <shared/currentTime/Local.h>
#include <shared/ServiceLocator.h>
#include "tools/OperatingSystemIntegration.h"

//define the main entry point
POCO_SERVER_MAIN(CYadomsServer)

shared::currentTime::Provider timeProvider(boost::make_shared<shared::currentTime::Local>());

CYadomsServer::CYadomsServer()
:m_helpRequested(false), 
m_startupOptions(new startupOptions::CStartupOptions(config())),
m_eventHandler(new shared::event::CEventHandler())
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

   logConfiguration::CLogConfiguration::configure(m_startupOptions->getLogLevel());
}

void CYadomsServer::uninitialize()
{
   YADOMS_LOG(information) << "Yadoms is shutting down";
   ServerApplication::uninitialize();

   //notify listeners that application ends (mostly used for application control handler (service and control) managers
   if (m_stopHandler)
      m_stopHandler->NotifyApplicationEnds();
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

void CYadomsServer::handleHelp(const std::string& name, const std::string& value)
{
   m_helpRequested = true;
   displayHelp();
   stopOptionsProcessing();
}

void CYadomsServer::displayHelp()
{
   Poco::Util::HelpFormatter helpFormatter(options());
   helpFormatter.setUnixStyle(true);
   helpFormatter.setCommand(commandName());
   helpFormatter.setUsage("OPTIONS");
   helpFormatter.setHeader("Yadoms command line usage");
   helpFormatter.format(std::cout);
}

int CYadomsServer::main(const Poco::Util::Application::ArgVec& args)
{
   if (!m_helpRequested)
   {
      std::string executablePath = config().getString("application.path");
      m_runningInformation.reset(new CRunningInformation(executablePath));



      YADOMS_LOG_CONFIGURE("Main");

      YADOMS_LOG(information) << "********************************************************************";
      YADOMS_LOG(information) << "Yadoms is starting";
      YADOMS_LOG(information) << "********************************************************************";
      YADOMS_LOG(information) << "Startup options :";
      YADOMS_LOG(information) << "\tlog level = " << m_startupOptions->getLogLevel();
      YADOMS_LOG(information) << "\tWeb server port number = " << m_startupOptions->getWebServerPortNumber();
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
      }


      if (m_startupOptions->getNoPasswordFlag())
         YADOMS_LOG(information) << "\tnoPassword = true";
      YADOMS_LOG(information) << "********************************************************************";

      //register Services in serviceLocator
      shared::CServiceLocator::instance().push<startupOptions::IStartupOptions>(m_startupOptions);
      shared::CServiceLocator::instance().push<IRunningInformation>(m_runningInformation);

      //configure the Poco ErrorHandler
      CErrorHandler eh;
      Poco::ErrorHandler* pOldEH = Poco::ErrorHandler::set(&eh);

      //configure stop handler
      m_stopHandler.reset(new CApplicationStopHandler(m_eventHandler, kTerminationRequested));
      shared::CServiceLocator::instance().push<IApplicationStopHandler>(m_stopHandler);

      tools::COperatingSystemIntegration integration;
      integration.configure();

      //create supervisor
      CSupervisor supervisor(m_eventHandler, kSupervisorIsStopped);
      Poco::Thread supervisorThread("Supervisor");
      supervisorThread.start(supervisor);

      bool stillRunning = true;
      while (stillRunning)
      {
         //configure console and ctrl+c handlers
         switch (m_eventHandler->waitForEvents())
         {
         case kTerminationRequested:
            //this event is launch by application stop handler
            //then just ask supervisor to stop and follow termination process
            YADOMS_LOG(debug) << "Receive termination request : ask supervisor to stop...";
            supervisor.requestToStop();
            YADOMS_LOG(debug) << "Supervisor stop asked";
            break;
         case kSupervisorIsStopped:
            YADOMS_LOG(debug) << "Receive supervisor stop signal";
            stillRunning = false;
            break;
         default:
            break;
         }
      }
      YADOMS_LOG(debug) << "Wait for supervisor finish...";
      supervisorThread.join();
      YADOMS_LOG(debug) << "Supervisor finished";

      //restore Poco ErrorHandler
      Poco::ErrorHandler::set(pOldEH);
   }
   YADOMS_LOG(debug) << "Yadoms is stopping, wait for supervisor finish...";
   return Poco::Util::Application::EXIT_OK;
}

