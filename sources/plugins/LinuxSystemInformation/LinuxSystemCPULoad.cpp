#include "stdafx.h"
#include <shared/Log.h>
#include "LinuxSystemCPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CLinuxSystemCPULoad::CLinuxSystemCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0), m_Capacity("cpuload"), m_Keyword("LinuxCPULoad")
{
   FILE* file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %Ld %Ld %Ld %Ld", &lastTotalUser, &lastTotalUserLow,
      &lastTotalSys, &lastTotalIdle);
   fclose(file);
}

CLinuxSystemCPULoad::~CLinuxSystemCPULoad()
{
}

const std::string& CLinuxSystemCPULoad::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CLinuxSystemCPULoad::getCapacity() const
{
   return m_Capacity;
}

const std::string& CLinuxSystemCPULoad::getKeyword() const
{
   return m_Keyword;
}

void CLinuxSystemCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareCustomKeyword(m_deviceId, getKeyword()  , getCapacity(), yApi::kGet , yApi::kNumeric, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CLinuxSystemCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_deviceId, getKeyword(), m_CPULoad);
}

double CLinuxSystemCPULoad::getValue()
{
   //TODO : Keep the last value, if an overflow occured
   double percent;
   unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;
   FILE* file;

   file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %Ld %Ld %Ld %Ld", &totalUser, &totalUserLow,
      &totalSys, &totalIdle);
   fclose(file);

   if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
      totalSys < lastTotalSys || totalIdle < lastTotalIdle){
         //Overflow detection. Just skip this value.
         percent = -1.0;
   }
   else{
      total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
         (totalSys - lastTotalSys);
      percent = total;
      total += (totalIdle - lastTotalIdle);
      percent /= total;
      percent *= 100;
   }

   lastTotalUser = totalUser;
   lastTotalUserLow = totalUserLow;
   lastTotalSys = totalSys;
   lastTotalIdle = totalIdle;

   m_CPULoad = percent;

   return percent;
}


