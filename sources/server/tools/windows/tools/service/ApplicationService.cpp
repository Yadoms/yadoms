/****************************** Module Header ******************************\
* Module Name:  SampleService.cpp
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
* 
* Provides a sample service class that derives from the service base class - 
* CServiceBase. The sample service logs the service start and stop 
* information to the Application event log, and shows how to run the main 
* function of the service in a thread pool worker thread.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/
#include "stdafx.h"
#include "ApplicationService.h"
#include "ThreadPool.h"
#include <Windows.h>
#include <shared/Log.h>

namespace tools { namespace service {

   CApplicationService::CApplicationService(IApplication & app, const std::string & pszServiceName,
                                  boost::filesystem::path path,
                                  BOOL fCanStop, 
                                  BOOL fCanShutdown, 
                                  BOOL fCanPauseContinue)
                                  : CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue), m_path(path), m_app(app), m_serviceName(pszServiceName), m_argc(0), m_argv(NULL)
   {
       m_fStopping = FALSE;
   }


   CApplicationService::~CApplicationService(void)
   {
   }


   //
   //   FUNCTION: CApplicationService::OnStart(DWORD, LPWSTR *)
   //
   //   PURPOSE: The function is executed when a Start command is sent to the 
   //   service by the SCM or when the operating system starts (for a service 
   //   that starts automatically). It specifies actions to take when the 
   //   service starts. In this code sample, OnStart logs a service-start 
   //   message to the Application log, and queues the main service function for 
   //   execution in a thread pool worker thread.
   //
   //   PARAMETERS:
   //   * dwArgc   - number of command line arguments
   //   * lpszArgv - array of command line arguments
   //
   //   NOTE: A service application is designed to be long running. Therefore, 
   //   it usually polls or monitors something in the system. The monitoring is 
   //   set up in the OnStart method. However, OnStart does not actually do the 
   //   monitoring. The OnStart method must return to the operating system after 
   //   the service's operation has begun. It must not loop forever or block. To 
   //   set up a simple monitoring mechanism, one general solution is to create 
   //   a timer in OnStart. The timer would then raise events in your code 
   //   periodically, at which time your service could do its monitoring. The 
   //   other solution is to spawn a new thread to perform the main service 
   //   functions, which is demonstrated in this code sample.
   //
   void CApplicationService::OnStart(DWORD dwArgc, char**lpszArgv)
   {
      m_argc = dwArgc;
      m_argv = lpszArgv;
   
       // Log a service start message to the Application log.
      WriteEventLogEntry((char*)(boost::format("%1% is starting") % m_serviceName).str().c_str(),  EVENTLOG_INFORMATION_TYPE);

       // Queue the main service function for execution in a worker thread.
       CThreadPool::QueueUserWorkItem(&CApplicationService::ServiceWorkerThread, this);
   }


   //
   //   FUNCTION: CApplicationService::ServiceWorkerThread(void)
   //
   //   PURPOSE: The method performs the main function of the service. It runs 
   //   on a thread pool worker thread.
   //
   void CApplicationService::ServiceWorkerThread(void)
   {
      boost::filesystem::current_path(m_path.string().c_str());
      m_app.run(m_argc, m_argv);

   }


   //
   //   FUNCTION: CApplicationService::OnStop(void)
   //
   //   PURPOSE: The function is executed when a Stop command is sent to the 
   //   service by SCM. It specifies actions to take when a service stops 
   //   running. In this code sample, OnStop logs a service-stop message to the 
   //   Application log, and waits for the finish of the main service function.
   //
   //   COMMENTS:
   //   Be sure to periodically call ReportServiceStatus() with 
   //   SERVICE_STOP_PENDING if the procedure is going to take long time. 
   //
   void CApplicationService::OnStop()
   {
       // Log a service stop message to the Application log.
      WriteEventLogEntry((char*)(boost::format("%1% is stopping") % m_serviceName).str().c_str(), EVENTLOG_INFORMATION_TYPE);


       // Indicate that the service is stopping and wait for the finish of the 
       // main service function (ServiceWorkerThread).
       boost::function<void()> callback = boost::bind(&CApplicationService::OnAppStopped, this);
       m_app.stop(callback);

       m_fStopping = TRUE;

       switch (m_eventHandler.waitForEvents())
       {
       case kAppEnd:
          //the app end normally
         {
            std::string msg = (boost::format("%1% is stopped") % m_serviceName).str();
            WriteEventLogEntry((char*)msg.c_str(), EVENTLOG_INFORMATION_TYPE);
            YADOMS_LOG(info) << msg;
            break;
         }

       default:
          {
             std::string msg = (boost::format("%1% fail to stopped") % m_serviceName).str();
             WriteEventLogEntry((char*)msg.c_str(), EVENTLOG_ERROR_TYPE);
             YADOMS_LOG(error) << msg;
             break;
          }
       }


   }
   

   void CApplicationService::OnAppStopped()
   {
      m_eventHandler.postEvent(kAppEnd);

   }
} //namespace service
} //namespace tools
   