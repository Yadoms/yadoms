#include "stdafx.h"
#include "Application.h"
#include "logConfiguration/LogConfiguration.h"
#include "startupOptions/Loader.h"
#include <shared/Log.h>
#include <tools/OperatingSystem.h>

CApplication::CApplication()
{
}
   

CApplication::~CApplication()
{
   if (!!m_supervisor)
   {
      switch(m_supervisor->stopMode())
      {
      case IApplicationStopHandler::kStopSystem: tools::COperatingSystem::shutdown(false); break;
      case IApplicationStopHandler::kRestartSystem: tools::COperatingSystem::shutdown(true); break;
      default: break;
      }
   }
}
   
void CApplication::configure(int argc, char ** argv)
{
   m_startupOptions.reset(new startupOptions::CLoader(argc, argv));

   logConfiguration::CLogConfiguration::configure(m_startupOptions->getLogLevel());
}

int CApplication::run()
{
  try
   {

      YADOMS_LOG_CONFIGURE("Main");

      YADOMS_LOG(information) << "********************************************************************";
      YADOMS_LOG(information) << "Yadoms is starting";
      YADOMS_LOG(information) << "********************************************************************";
      YADOMS_LOG(information) << "Startup options :";
      YADOMS_LOG(information) << "\tlog level = " << m_startupOptions->getLogLevel();
      YADOMS_LOG(information) << "\tWeb server port number = " << m_startupOptions->getWebServerPortNumber();
      YADOMS_LOG(information) << "\tWeb server ip = " << m_startupOptions->getWebServerIPAddress();
      YADOMS_LOG(information) << "\tWeb server path = " << m_startupOptions->getWebServerInitialPath();
      YADOMS_LOG(information) << "\tdb path = " << m_startupOptions->getDatabaseFile();
      YADOMS_LOG(information) << "\tplugins path = " << m_startupOptions->getPluginsPath();
      YADOMS_LOG(information) << "\twidgets path = " << m_startupOptions->getWidgetsPath();
      YADOMS_LOG(information) << "********************************************************************";

      startupOptions::IStartupOptions & opt = *m_startupOptions.get();

      m_supervisor.reset(new CSupervisor(opt));

      // The main job
      m_supervisor->doWork();

      YADOMS_LOG(information) << "Yadoms is stopped ";
   }
   catch(startupOptions::CLoaderException& e)
   {
      if (e.isError())
      {
         YADOMS_LOG(fatal) << e.what();
      }
      else
      {
         // Help was invoked, just print to console
         YADOMS_LOG(information) << e.what();
      }
   }
   catch(...)
   {
      //dual logging in case logger fails/throws
      std::cout << "An unhandled exception occurred. Yadoms is now stopped" << std::endl;
      YADOMS_LOG(fatal) << "An unhandled exception occurred. Yadoms is now stopped";
      BOOST_ASSERT_MSG(false, "Yadoms exited with error, see console for details");
   }

   return 0;
}

void CApplication::stop(boost::function<void()> & callbackAfterStopped)
{
   if (m_supervisor)
   {
      m_supervisor->requestToStop(callbackAfterStopped);
   }
}

