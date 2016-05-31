#include "stdafx.h"
#include "ApplicationStopHandler.h"

namespace shared
{
   namespace process
   {
      boost::function<bool()> CConsoleControlHandler::m_onStopRequestedFct;
      
      void crashHandler(int signal)
      {
         void *array[10];
         size_t size;

         // get void*'s for all entries on the stack
         size = backtrace(array, 10);

         // print out all the frames to stderr
         fprintf(stderr, "Error: signal %d:\n", signal);
         backtrace_symbols_fd(array, size, STDERR_FILENO);
         exit(1);
      }

      void stopHandler(int signal)
      {
         switch(signal)
         {
         case  SIGINT :
         case  SIGTERM :
            // Signal stop request and wait for application fully stops
            if (!CConsoleControlHandler::m_onStopRequestedFct())
               YADOMS_LOG(error) << "Fail to wait the app end event";
            break;
         default:
            YADOMS_LOG(warning) << "CApplicationStopHandler::handleInternal - no handler for #%d signal " << signal;
            break;
         }
      }
   
      CApplicationStopHandler::CApplicationStopHandler(bool isRunningAsService)
      {
      }

      CApplicationStopHandler::~CApplicationStopHandler()
      {
      }

      void CApplicationStopHandler::setApplicationStopHandler(boost::function<bool()> onStopRequestedFct)
      {
         m_onStopRequestedFct = onStopRequestedFct;
         
         signal(SIGSEGV, crashHandler);   // install our handler

         signal(SIGINT, stopHandler);   // CTRL+C signal
         signal(SIGTERM, stopHandler);   // Termination request
      }
   }
} // namespace shared::process
