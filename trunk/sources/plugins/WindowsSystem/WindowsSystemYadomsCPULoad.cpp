#include "stdafx.h"
#include "WindowsSystemYadomsCPULoad.h"
#include <shared/exception/Exception.hpp>

CWindowsSystemYadomsCPULoad::CWindowsSystemYadomsCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0)
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
            throw shared::exception::CException ( Message.str() );
        }
        memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
        memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));   
}

CWindowsSystemYadomsCPULoad::~CWindowsSystemYadomsCPULoad()
{}

const std::string& CWindowsSystemYadomsCPULoad::getDeviceId() const
{
   return m_deviceId;
}

double CWindowsSystemYadomsCPULoad::getValue()
{
        FILETIME ftime, fsys, fuser;
        unsigned __int64 now, sys, user;
        double percent;
    
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