#include "stdafx.h"
#include "WindowsSystemDisksList.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CWindowsSystemDisksList::CWindowsSystemDisksList(void)
{
   DWORD cchBuffer;
   LPSTR driveStrings, driveStringsInit;
   UINT driveType;

   // Find out how big a buffer we need
   cchBuffer = GetLogicalDriveStrings(0, NULL);

   driveStrings = (LPSTR)malloc((cchBuffer + 1) * sizeof(TCHAR));
   driveStringsInit = driveStrings;

   if (driveStrings == NULL || cchBuffer == NULL)
   {
      std::stringstream Message;
      Message << "Cannot allocate memory for CWindowsSystemDisksList : ";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   // Fetch all drive strings    
   if (GetLogicalDriveStrings(cchBuffer, driveStrings)==NULL)
   {
      std::stringstream Message;
      Message << "GetLogicalDriveStrings return an error : ";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   while (*driveStrings)
   {
      driveType = GetDriveType(driveStrings);
      if (driveType == DRIVE_FIXED)
      {
         DrivesList.push_back( driveStrings );
      }
      driveStrings += lstrlen(driveStrings) + 1;
   }

   free(driveStringsInit);
}

CWindowsSystemDisksList::~CWindowsSystemDisksList()
{}

std::vector<std::string>& CWindowsSystemDisksList::getList()
{
   return DrivesList;
}


