#include "stdafx.h"
#include "WindowsSystemDiskUsage.h"
#include <shared/exception/Exception.hpp>

CWindowsSystemDiskUsage::CWindowsSystemDiskUsage(const std::string & deviceId, const std::string & driveName)
   :m_deviceId(deviceId), m_driveName(driveName)
{
}

CWindowsSystemDiskUsage::~CWindowsSystemDiskUsage()
{}

const std::string& CWindowsSystemDiskUsage::getDeviceId() const
{
   return m_deviceId;
}

double CWindowsSystemDiskUsage::getValue()
{
   ULARGE_INTEGER FreeSpaceAvailable, TotalSpace, TotalFreeSpaceAvailable;

   if (!GetDiskFreeSpaceEx(m_driveName.c_str(), &FreeSpaceAvailable, &TotalSpace, &TotalFreeSpaceAvailable))
   {
      std::stringstream Message;
      Message << "GetDiskFreeSpaceEx return an error : ";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   return (1 - (double) FreeSpaceAvailable.QuadPart / TotalSpace.QuadPart) * 100;
}


