#include "stdafx.h"
#include "DisksList.h"
#include <shared/exception/Exception.hpp>

CDisksList::CDisksList()
{
   // Find out how big a buffer we need
   auto cchBuffer = GetLogicalDriveStrings(0, nullptr);

   auto driveStrings = static_cast<LPWSTR>(malloc((cchBuffer + 1) * sizeof(WCHAR)));
   auto driveStringsInit = driveStrings;

   if (driveStrings == nullptr || cchBuffer == NULL)
   {
      std::stringstream Message;
      Message << "Cannot allocate memory for CDisksList : ";
      Message << GetLastError();
      throw shared::exception::CException(Message.str());
   }

   // Fetch all drive strings    
   if (GetLogicalDriveStrings(cchBuffer, driveStrings) == NULL)
   {
      std::stringstream Message;
      Message << "GetLogicalDriveStrings return an error : ";
      Message << GetLastError();
      throw shared::exception::CException(Message.str());
   }

   while (*driveStrings)
   {
      if (GetDriveType(driveStrings) == DRIVE_FIXED)
      {
         std::wstring wDriveStrings(driveStrings);
         DrivesList.push_back(std::string(wDriveStrings.begin(), wDriveStrings.end()));
      }
      driveStrings += lstrlen(driveStrings) + 1;
   }

   free(driveStringsInit);
}

CDisksList::~CDisksList()
{
}

const std::vector<std::string>& CDisksList::getList() const
{
   return DrivesList;
}

