/****************************** Module Header ******************************\
* Module Name:  ServiceInstaller.cpp
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
*
* The file implements functions that install and uninstall the service.
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
#include <stdio.h>
#include <windows.h>
#include "ServiceInstaller.h"
#include <shared/Log.h>

namespace tools { namespace service {

   //
   //   FUNCTION: InstallService
   //
   //   PURPOSE: Install the current application as a service to the local 
   //   service control manager database.
   //
   //   PARAMETERS:
   //   * pszServiceName - the name of the service to be installed
   //   * pszDisplayName - the display name of the service
   //   * dwStartType - the service start option. This parameter can be one of 
   //     the following values: SERVICE_AUTO_START, SERVICE_BOOT_START, 
   //     SERVICE_DEMAND_START, SERVICE_DISABLED, SERVICE_SYSTEM_START.
   //   * pszDependencies - a pointer to a double null-terminated array of null-
   //     separated names of services or load ordering groups that the system 
   //     must start before this service.
   //   * pszAccount - the name of the account under which the service runs.
   //   * pszPassword - the password to the account name.
   //
   //   NOTE: If the function fails to install the service, it prints the error 
   //   in the standard output stream for users to diagnose the problem.
   //
   void CServiceInstaller::InstallService(const std::string & pszServiceName,
      const std::string &  pszDisplayName,
      EServiceStartType startType,
      const std::string &  pszDependencies,
      const std::string &  pszAccount,
      const std::string &  pszPassword,
      const std::string &  serviceArguments)
   {
      SC_HANDLE schSCManager = NULL;
      SC_HANDLE schService = NULL;

      try
      {
         char szPath[MAX_PATH];

         if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)) == 0)
         {
            std::stringstream ss;
            ss << "GetModuleFileName failed w/err 0x";
            ss << std::hex << std::setw(8) << std::setfill('0') << GetLastError();
            throw std::exception(ss.str().c_str());

         }
         std::string commandlineService;

         commandlineService += "\"";
         commandlineService += std::string(szPath);
         commandlineService += "\"";
         if (!serviceArguments.empty())
         {
            commandlineService += " ";
            commandlineService += serviceArguments;
         }

         // Open the local default service control manager database
         schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT |
            SC_MANAGER_CREATE_SERVICE);
         if (schSCManager == NULL)
         {
            std::stringstream ss;
            ss << "OpenSCManager failed w/err 0x";
            ss << std::hex << std::setw(8) << std::setfill('0') << GetLastError();
            throw std::exception(ss.str().c_str());
         }

         // Install the service into SCM by calling CreateService
         schService = CreateService(
            schSCManager,                   // SCManager database
            pszServiceName.c_str(),         // Name of service
            pszDisplayName.c_str(),         // Name to display
            SERVICE_QUERY_STATUS,           // Desired access
            SERVICE_WIN32_OWN_PROCESS,      // Service type
            startType,                      // Service start type
            SERVICE_ERROR_NORMAL,           // Error control type
            commandlineService.c_str(),     // Service's binary
            NULL,                           // No load ordering group
            NULL,                           // No tag identifier
            pszDependencies.c_str(),        // Dependencies
            pszAccount.c_str(),             // Service running account
            pszPassword.c_str()             // Password of the account
            );

         if (schService == NULL)
         {
            std::stringstream ss;

            switch (GetLastError())
            {
            case ERROR_ACCESS_DENIED:
               ss << "The handle to the SCM database does not have the SC_MANAGER_CREATE_SERVICE access right.";
               break;
            case ERROR_CIRCULAR_DEPENDENCY:
               ss << "A circular service dependency was specified.";
               break;
            case ERROR_DUPLICATE_SERVICE_NAME:
               ss << "The display name already exists in the service control manager database either as a service name or as another display name.";
               break;
            case ERROR_INVALID_HANDLE:
               ss << "The handle to the specified service control manager database is invalid.";
               break;
            case ERROR_INVALID_NAME:
               ss << "The specified service name is invalid.";
               break;
            case ERROR_INVALID_PARAMETER:
               ss << "A parameter that was specified is invalid.";
               break;
            case ERROR_INVALID_SERVICE_ACCOUNT:
               ss << "The user account name specified in the lpServiceStartName parameter does not exist.";
               break;
            case ERROR_SERVICE_EXISTS:
               ss << "The specified service already exists in this database.";
               break;
            case ERROR_SERVICE_MARKED_FOR_DELETE:
               ss << "The specified service already exists in this database and has been marked for deletion.";
               break;
            default:
               ss << "CreateService failed w/err 0x";
               ss << std::hex << std::setw(8) << std::setfill('0') << GetLastError();

               break;
            }



            throw std::exception(ss.str().c_str());

         }

         YADOMS_LOG(info) << pszServiceName << " is installed.";
      }
      catch (std::exception &ex)
      {
         YADOMS_LOG(error) << ex.what();
      }

      // Centralized cleanup for all allocated resources.
      if (schSCManager)
      {
         CloseServiceHandle(schSCManager);
         schSCManager = NULL;
      }
      if (schService)
      {
         CloseServiceHandle(schService);
         schService = NULL;
      }



   }


   //
   //   FUNCTION: UninstallService
   //
   //   PURPOSE: Stop and remove the service from the local service control 
   //   manager database.
   //
   //   PARAMETERS: 
   //   * pszServiceName - the name of the service to be removed.
   //
   //   NOTE: If the function fails to uninstall the service, it prints the 
   //   error in the standard output stream for users to diagnose the problem.
   //
   void CServiceInstaller::UninstallService(const std::string &  pszServiceName)
   {
      SC_HANDLE schSCManager = NULL;
      SC_HANDLE schService = NULL;
      try
      {
         SERVICE_STATUS ssSvcStatus = {};

         // Open the local default service control manager database
         schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
         if (schSCManager == NULL)
         {
            std::stringstream ss;
            ss << "OpenSCManager failed w/err 0x";
            ss << std::hex << std::setw(8) << std::setfill('0') << GetLastError();
            throw std::exception(ss.str().c_str());
         }

         // Open the service with delete, stop, and query status permissions
         schService = OpenService(schSCManager, pszServiceName.c_str(), SERVICE_STOP |
            SERVICE_QUERY_STATUS | DELETE);
         if (schService == NULL)
         {
            std::stringstream ss;
            ss << "OpenService failed w/err 0x";
            ss << std::hex << std::setw(8) << std::setfill('0') << GetLastError();
            throw std::exception(ss.str().c_str());
         }

         // Try to stop the service
         if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
         {
            YADOMS_LOG(info) << "Stopping " << pszServiceName;
            Sleep(1000);

            while (QueryServiceStatus(schService, &ssSvcStatus))
            {
               if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
               {
                  YADOMS_LOG(info) << "(waiting)";
                  Sleep(1000);
               }
               else break;
            }

            if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
            {
               YADOMS_LOG(info) << pszServiceName << " is stopped.";
            }
            else
            {
               YADOMS_LOG(error) << pszServiceName << " failed to stop.";
            }
         }

         // Now remove the service by calling DeleteService.
         if (!DeleteService(schService))
         {
            std::stringstream ss;
            ss << "DeleteService failed w/err 0x";
            ss << std::hex << std::setw(8) << std::setfill('0') << GetLastError();
            throw std::exception(ss.str().c_str());

         }

         YADOMS_LOG(info) << pszServiceName << " is removed.";
      }
      catch (std::exception & ex)
      {
         YADOMS_LOG(error) << ex.what();
      }

      // Centralized cleanup for all allocated resources.
      if (schSCManager)
      {
         CloseServiceHandle(schSCManager);
         schSCManager = NULL;
      }
      if (schService)
      {
         CloseServiceHandle(schService);
         schService = NULL;
      }
   }


} //namespace service
} //namespace tools