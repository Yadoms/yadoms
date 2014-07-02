#include "stdafx.h"
#include "WindowsSystemMemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CWindowsSystemMemoryLoad::CWindowsSystemMemoryLoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_memoryLoad(0), m_Capacity("MemoryLoad"), m_Keyword("WindowsMemoryLoad")
{}

CWindowsSystemMemoryLoad::~CWindowsSystemMemoryLoad()
{}

const std::string& CWindowsSystemMemoryLoad::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CWindowsSystemMemoryLoad::getCapacity() const
{
   return m_Capacity;
}

const std::string& CWindowsSystemMemoryLoad::getKeyword() const
{
   return m_Keyword;
}

void CWindowsSystemMemoryLoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareCustomKeyword(m_deviceId, getKeyword(), getCapacity(), yApi::kReadOnly, yApi::kDecimal, yApi::CStandardUnits::Percent);
}

void CWindowsSystemMemoryLoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, getKeyword()  , m_memoryLoad);
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


