#include "stdafx.h"
#include "ApplicationStopHandler.h"
#include <execinfo.h>
#include <shared/Log.h>
#include <shared/process/SoftwareStop.h>

namespace shared
{
   namespace process
   {
      boost::function<bool()> CApplicationStopHandler::m_onStopRequestedFct;

      void CApplicationStopHandler::crashHandler(int signal)
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

      void CApplicationStopHandler::stopHandler(int signal)
      {
         switch(signal)
         {
         case SIGINT :
         case SIGTERM :
	         {
		         // Linux stop handler must not block, so execute handler in separate thread
		         boost::thread asyncHandlerThread([]()
				      {
							// Signal stop request and wait for application fully stops
							if (!CApplicationStopHandler::m_onStopRequestedFct.empty() && !CApplicationStopHandler::m_onStopRequestedFct())
								YADOMS_LOG(error) << "CApplicationStopHandler : Fail to wait the app end event";
				      });
		         break;
            }
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
         m_onStopRequestedFct = boost::function<bool()>();
      }

      void CApplicationStopHandler::setApplicationStopHandler(boost::function<bool()> onStopRequestedFct)
      {
         m_onStopRequestedFct = onStopRequestedFct;

         CSoftwareStop::setOnStopRequestedHandler(onStopRequestedFct);

         signal(SIGSEGV, CApplicationStopHandler::crashHandler);   // crash handler

         if (m_onStopRequestedFct.empty())
         {
            signal(SIGINT, SIG_IGN);     // CTRL+C signal
            signal(SIGTERM, SIG_IGN);    // Termination request
         }
         else
         {
            signal(SIGINT, CApplicationStopHandler::stopHandler);     // CTRL+C signal
            signal(SIGTERM, CApplicationStopHandler::stopHandler);    // Termination request
         }
      }
   }
} // namespace shared::process

