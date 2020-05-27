#include "stdafx.h"
#include "ServiceControlHandler.h"
#include <windows.h> 
#include <shared/Log.h>

namespace shared
{
   namespace process
   {
      boost::function<bool()> CServiceControlHandler::m_onStopRequestedFct;

      void CServiceControlHandler::setOnStopRequestedHandler(boost::function<bool()> onStopRequestedFct)
      {
         m_onStopRequestedFct = onStopRequestedFct;

         m_serviceStatusHandle = RegisterServiceCtrlHandler("", reinterpret_cast<LPHANDLER_FUNCTION>(ServiceControlHandler));
      }

      SERVICE_STATUS CServiceControlHandler::m_serviceStatus;
      SERVICE_STATUS_HANDLE CServiceControlHandler::m_serviceStatusHandle;

      void CServiceControlHandler::ServiceControlHandler(DWORD control)
      {
         switch (control)
         {
         case SERVICE_CONTROL_STOP:
         case SERVICE_CONTROL_SHUTDOWN:
            {
               // Signal stop request and wait for application fully stops
               if (!m_onStopRequestedFct())
                  YADOMS_LOG(error) << "CServiceControlHandler : Fail to wait the app end event";

               m_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            }
            break;
         case SERVICE_CONTROL_INTERROGATE:
         default:
            break;
         }
         SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus);
      }
   }
} // namespace shared::process


