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

      void CApplicationStopHandler::stopHandler(int signal)
      {
         switch(signal)
         {
         case  SIGINT :
         case  SIGTERM :
            // Signal stop request and wait for application fully stops
            if (!CApplicationStopHandler::m_onStopRequestedFct())
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

         CSoftwareStop::setOnStopRequestedHandler(onStopRequestedFct);

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
