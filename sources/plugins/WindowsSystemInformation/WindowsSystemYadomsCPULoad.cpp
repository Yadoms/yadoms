#include "stdafx.h"
#include "WindowsSystemYadomsCPULoad.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CWindowsSystemYadomsCPULoad::CWindowsSystemYadomsCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0), m_Capacity("cpuload"), m_Keyword("YadomsCPULoad"), m_lastCPU(0), m_lastSysCPU(0), m_lastUserCPU(0), m_numProcessors(0), m_InitializeOk(false)
{}

void CWindowsSystemYadomsCPULoad::Initialize()
{
   SYSTEM_INFO sysInfo;
   FILETIME ftime, fsys, fuser;

   GetSystemInfo(&sysInfo);
   m_numProcessors = sysInfo.dwNumberOfProcessors;

   GetSystemTimeAsFileTime(&ftime);
   memcpy(&m_lastCPU, &ftime, sizeof(FILETIME));

   m_self = GetCurrentProcess();
   if (!GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser))
   {
      std::stringstream Message;
      Message << "Fail to retrieve Process Times with the error :";
      Message << GetLastError();
      m_InitializeOk = false;
      throw shared::exception::CException ( Message.str() );
   }
   memcpy(&m_lastSysCPU, &fsys, sizeof(FILETIME));
   memcpy(&m_lastUserCPU, &fuser, sizeof(FILETIME));

   m_InitializeOk = true;
}

CWindowsSystemYadomsCPULoad::~CWindowsSystemYadomsCPULoad()
{}

const std::string& CWindowsSystemYadomsCPULoad::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CWindowsSystemYadomsCPULoad::getCapacity() const
{
   return m_Capacity;
}

const std::string& CWindowsSystemYadomsCPULoad::getKeyword() const
{
   return m_Keyword;
}

void CWindowsSystemYadomsCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   if (m_InitializeOk)
   {
      // Declare the device
      context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

      // Declare associated keywords (= values managed by this device)
      context->declareCustomKeyword(m_deviceId, getKeyword(), getCapacity(), yApi::kGet, yApi::kNumeric, yApi::CStandardUnits::Percent);
   }
}

void CWindowsSystemYadomsCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   if (m_InitializeOk)
      context->historizeData(m_deviceId, getKeyword()  , m_CPULoad);
}

double CWindowsSystemYadomsCPULoad::getValue()
{
   FILETIME ftime, fsys, fuser;
   unsigned __int64 now, sys, user;

   if (m_InitializeOk)
   {
      GetSystemTimeAsFileTime(&ftime);
      memcpy(&now, &ftime, sizeof(FILETIME));

      if (!GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser))
      {
         std::stringstream Message;
         Message << "Fail to retrieve Process Times with the error :";
         Message << GetLastError();
         throw shared::exception::CException ( Message.str() );
      }

      memcpy(&sys, &fsys, sizeof(FILETIME));
      memcpy(&user, &fuser, sizeof(FILETIME));
      double percent = double((sys - m_lastSysCPU) + (user - m_lastUserCPU)) / (now - m_lastCPU);
      percent /= m_numProcessors;
      m_lastCPU = now;
      m_lastUserCPU = user;
      m_lastSysCPU = sys;

      m_CPULoad = percent * 100;

      return m_CPULoad;
   }
   else
   {
      YADOMS_LOG(trace) << getDeviceId() << " is desactivated";
      return 0;
   }
}
