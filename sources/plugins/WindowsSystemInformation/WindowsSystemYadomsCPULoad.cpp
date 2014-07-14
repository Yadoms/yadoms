#include "stdafx.h"
#include "WindowsSystemYadomsCPULoad.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CWindowsSystemYadomsCPULoad::CWindowsSystemYadomsCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0), m_Capacity("cpuload"), m_Keyword("YadomsCPULoad")
{}

void CWindowsSystemYadomsCPULoad::Initialize()
{
        SYSTEM_INFO sysInfo;
        FILETIME ftime, fsys, fuser;
    
        GetSystemInfo(&sysInfo);
        numProcessors = sysInfo.dwNumberOfProcessors;
    
        GetSystemTimeAsFileTime(&ftime);
        memcpy(&lastCPU, &ftime, sizeof(FILETIME));
    
        self = GetCurrentProcess();
        if (!GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser))
        {
            std::stringstream Message;
            Message << "Fail to retrieve Process Times with the error :";
            Message << GetLastError();
			m_InitializeOk = false;
            throw shared::exception::CException ( Message.str() );
        }
        memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
        memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

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
        double percent;
 
   if (m_InitializeOk)
   {
        GetSystemTimeAsFileTime(&ftime);
        memcpy(&now, &ftime, sizeof(FILETIME));
    
        if (!GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser))
        {
            std::stringstream Message;
            Message << "Fail to retrieve Process Times with the error :";
            Message << GetLastError();
            throw shared::exception::CException ( Message.str() );
        }

        memcpy(&sys, &fsys, sizeof(FILETIME));
        memcpy(&user, &fuser, sizeof(FILETIME));
        percent = double((sys-lastSysCPU) + (user - lastUserCPU)) / (now - lastCPU);
        percent /= numProcessors;
        lastCPU = now;
        lastUserCPU = user;
        lastSysCPU = sys;
    
        return percent * 100;
   }
   else
   {
	   YADOMS_LOG(trace) << getDeviceId() << " is desactivated";
	   return 0;
   }
}