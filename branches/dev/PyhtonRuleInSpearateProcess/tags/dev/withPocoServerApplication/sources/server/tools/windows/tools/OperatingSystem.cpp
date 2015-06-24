#include "stdafx.h"
#include "OperatingSystem.h"
#include <windows.h>

namespace tools {

   bool COperatingSystem::shutdown(bool andRestart)
   {
      bool success = false;
      UINT exitWindowsFlags = EWX_FORCE | (andRestart ? EWX_REBOOT : EWX_SHUTDOWN);

      // This function opens the access token associated with a process.
      HANDLE handle;
      if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &handle))
      {
         // This function retrieves the locally unique identifier (LUID) used on a specified system to locally represent the specified privilege name.
         TOKEN_PRIVILEGES privileges = {0};
         privileges.PrivilegeCount = 1;
         privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
         if (LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid))
         {
            // This function enables or disables privileges in the specified access token.
            // Enabling or disabling privileges in an access token requires TOKEN_ADJUST_PRIVILEGES access.
            DWORD length = 0;
            if (AdjustTokenPrivileges(handle, FALSE, &privileges, 0, NULL, &length))
            {
               // Shuts down the system and reboots.
               // Shuts down the system to a point at which it is safe to turn off the power.
               // All file buffers have been flushed to disk, and all running processes have stopped. 
               if (ExitWindowsEx(exitWindowsFlags, 0))
                  success = true;
            }
         }
         CloseHandle (handle);
      }

      if (!success)
         ExitWindowsEx (exitWindowsFlags, 0);

      return success;
   }
   
} //namespace tools
