#include "stdafx.h"
#include "WindowsSystemYadomsCPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>

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

void CWindowsSystemYadomsCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   //TODO : A finaliser
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_deviceId, "temp1"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "temp2"  , yApi::CStandardCapacities::Temperature , yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "battery", yApi::CStandardCapacities::BatteryLevel, yApi::IYadomsApi::kReadOnly);
   //context->declareKeyword(m_deviceId, "Rssi"   , yApi::CStandardCapacities::Rssi        , yApi::IYadomsApi::kReadOnly);
}

void CWindowsSystemYadomsCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   //TODO : A finaliser
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "temp1"  , m_CPULoad);
   //context->historizeData(m_deviceId, "temp2"  , m_temperature2);
   //context->historizeData(m_deviceId, "battery", m_batteryLevel);
   //context->historizeData(m_deviceId, "Rssi"   , m_rssi        );
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