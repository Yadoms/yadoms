#include "stdafx.h"
#include "ConsoleControlHandler.h"
#include <windows.h> 
#include <shared/Log.h>

namespace tools {

   shared::event::CEventHandler CConsoleControlHandler::m_eventHandler;

   tools::IApplication * CConsoleControlHandler::m_pApplication = NULL;
   

   void CConsoleControlHandler::registerApplication(tools::IApplication & app)
   {
      m_pApplication = &app;
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
         if (m_pApplication != NULL)
         {
            boost::function<void()> callback = boost::bind(&CConsoleControlHandler::onAppStopped);
            m_pApplication->stop(callback);

            switch (m_eventHandler.waitForEvents())
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
            
         }
         return FALSE;        

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


   void CConsoleControlHandler::onAppStopped()
   {
      m_eventHandler.postEvent(kAppEnd);
   }




} //namespace tools
