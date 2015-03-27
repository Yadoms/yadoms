#include "stdafx.h"
#include "ConsoleControlHandler.h"
#include <windows.h> 
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"

namespace tools {

   boost::shared_ptr<shared::event::CEventHandler> CConsoleControlHandler::m_eventHandler(new shared::event::CEventHandler());

   void CConsoleControlHandler::ConfigureConsoleControl()
   {
      SetConsoleCtrlHandler((PHANDLER_ROUTINE)CConsoleControlHandler::ctrlHandler, TRUE);
   }

   BOOL CConsoleControlHandler::ctrlHandler(DWORD fdwCtrlType)
   {
      switch (fdwCtrlType)
      {
         // Handle the CTRL-C signal. 
      case CTRL_C_EVENT:
         // CTRL-CLOSE: confirm that the user wants to exit. 
      case CTRL_CLOSE_EVENT:
      {
         boost::shared_ptr<IApplicationStopHandler> stopHandler = shared::CServiceLocator::instance().get<IApplicationStopHandler>();
         if (stopHandler)
         {
            stopHandler->registerForAppEnds(m_eventHandler, kAppEnd);
            stopHandler->requestToStop(IApplicationStopHandler::kYadomsOnly);
         }

         //stay blocking until app is not stopped
         switch (m_eventHandler->waitForEvents())
         {
         case kAppEnd:
            //the app end normally
            return TRUE;
            break;

         default:
            YADOMS_LOG(error) << "Fail to wait the app end event";
            return FALSE;
            break;
         }
         return FALSE;
      }
         // Pass other signals to the next handler. 
      case CTRL_BREAK_EVENT:
         //printf( "Ctrl-Break event\n\n" );
         return FALSE;

      case CTRL_LOGOFF_EVENT:
         //printf( "Ctrl-Logoff event\n\n" );
         return FALSE;

      case CTRL_SHUTDOWN_EVENT:
         //printf( "Ctrl-Shutdown event\n\n" );
         return FALSE;

      default:
         return FALSE;
      }
   }


} //namespace tools
