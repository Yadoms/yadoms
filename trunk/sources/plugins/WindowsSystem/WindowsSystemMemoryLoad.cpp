#include "stdafx.h"
#include "WindowsSystemMemoryLoad.h"
#include <shared/exception/Exception.hpp>

CWindowsSystemMemoryLoad::CWindowsSystemMemoryLoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_memoryLoad(0)
{}

CWindowsSystemMemoryLoad::~CWindowsSystemMemoryLoad()
{}

const std::string& CWindowsSystemMemoryLoad::getDeviceId() const
{
   return m_deviceId;
}

double CWindowsSystemMemoryLoad::getValue()
{
   MEMORYSTATUSEX statex;

   statex.dwLength = sizeof (statex);

   if (GlobalMemoryStatusEx (&statex))
      m_memoryLoad = (double(statex.ullTotalPhys - statex.ullAvailPhys)*100 / statex.ullTotalPhys );
   else
   {
      std::stringstream Message;
      Message << "Fail to read Windows system memory size :";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   return m_memoryLoad;
}


