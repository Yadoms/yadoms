#include "stdafx.h"
#include "WindowsSystemMemoryLoad.h"
#include <shared/Log.h>

CWindowsSystemMemoryLoad::CWindowsSystemMemoryLoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_memoryLoad(0)
{
}

CWindowsSystemMemoryLoad::~CWindowsSystemMemoryLoad()
{
}

bool CWindowsSystemMemoryLoad::read()
{
   MEMORYSTATUSEX statex;

   statex.dwLength = sizeof (statex);

   if (GlobalMemoryStatusEx (&statex))
   {
      m_memoryLoad = (double(statex.ullTotalPhys - statex.ullAvailPhys)*100 / statex.ullTotalPhys );
      return true;
   }
   else
   {
      YADOMS_LOG(error) << "Fail to read Windows system memory size";
      return false;
   }
}

const std::string& CWindowsSystemMemoryLoad::getDeviceId() const
{
   return m_deviceId;
}

double CWindowsSystemMemoryLoad::getValue() const
{
   return m_memoryLoad;
}


