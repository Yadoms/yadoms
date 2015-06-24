/****************************** Module Header ******************************\
* Module Name:  SampleService.h
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

#pragma once
#include "ServiceBase.h"
#include "tools/IApplication.h"

namespace tools { namespace service {

   class CApplicationService : public CServiceBase
   {
   public:

       CApplicationService(IApplication & app, 
         const std::string & pszServiceName, 
          boost::filesystem::path path,
           BOOL fCanStop = TRUE, 
           BOOL fCanShutdown = TRUE, 
           BOOL fCanPauseContinue = FALSE);
       virtual ~CApplicationService(void);

   protected:

       virtual void OnStart(DWORD dwArgc, char **pszArgv);
       virtual void OnStop();

       void ServiceWorkerThread(void);

   private:
      void OnAppStopped();
       BOOL m_fStopping;
       HANDLE m_hStoppedEvent;

       boost::filesystem::path m_path;
       int m_argc;
       char ** m_argv;
       IApplication & m_app;
       std::string m_serviceName;
   };
   
} //namespace service
} //namespace tools