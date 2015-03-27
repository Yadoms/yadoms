#include "stdafx.h"
#include "ServiceControlHandler.h"
#include <windows.h> 
#include <shared/Log.h>
#include <shared/ServiceLocator.h>
#include "IApplicationStopHandler.h"
#include "startupOptions/IStartupOptions.h"

namespace tools {

   boost::shared_ptr<shared::event::CEventHandler> CServiceControlHandler::m_eventHandler(new shared::event::CEventHandler());

   SERVICE_STATUS CServiceControlHandler::_serviceStatus;
   SERVICE_STATUS_HANDLE  CServiceControlHandler::_serviceStatusHandle;

   void CServiceControlHandler::ConfigureServiceControl()
   {
      boost::shared_ptr<startupOptions::IStartupOptions> startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();
      if (startupOptions && startupOptions->getIsRunningAsService())
      {
#if defined(POCO_WIN32_UTF8) && !defined(POCO_NO_WSTRING)
         _serviceStatusHandle = RegisterServiceCtrlHandlerW(L"", (LPHANDLER_FUNCTION)ServiceControlHandler);
#else
         _serviceStatusHandle = RegisterServiceCtrlHandlerA("", ServiceControlHandler);
#endif
      }
   }

   void CServiceControlHandler::ServiceControlHandler(DWORD control)
   {

      switch (control)
      {
      case SERVICE_CONTROL_STOP:
      case SERVICE_CONTROL_SHUTDOWN:
         {
            YADOMS_LOG(information) << "Service is requested to stop";

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
               break;

            default:
               YADOMS_LOG(error) << "Fail to wait the app end event";
               break;
            }
            YADOMS_LOG(information) << "Service is nearly stopped";

            _serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
         }
         break;
      case SERVICE_CONTROL_INTERROGATE:
         break;
      }
      SetServiceStatus(_serviceStatusHandle, &_serviceStatus);
   }




} //namespace tools
