#include "stdafx.h"
#include "LinuxSystemYadomsCPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CLinuxSystemYadomsCPULoad::CLinuxSystemYadomsCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0)
{
   FILE* file;
   struct tms timeSample;
   char line[128];


   lastCPU = times(&timeSample);
   lastSysCPU = timeSample.tms_stime;
   lastUserCPU = timeSample.tms_utime;


   file = fopen("/proc/cpuinfo", "r");
   numProcessors = 0;
   while(fgets(line, 128, file) != NULL)
   {
      if (strncmp(line, "processor", 9) == 0) numProcessors++;
   }
   fclose(file);
}

CLinuxSystemYadomsCPULoad::~CLinuxSystemYadomsCPULoad()
{}

const std::string& CLinuxSystemYadomsCPULoad::getDeviceId() const
{
   return m_deviceId;
}

void CLinuxSystemYadomsCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   //context->declareKeyword(m_deviceId, "YadomsCPULoad"  , "cpuload" , yApi::IYadomsApi::kReadOnly , yApi::IYadomsApi::kDecimal, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CLinuxSystemYadomsCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "YadomsCPULoad"  , m_CPULoad);
}

double CLinuxSystemYadomsCPULoad::getValue()
{
   struct tms timeSample;
   clock_t now;
   double percent;
    

   now = times(&timeSample);
   if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
      timeSample.tms_utime < lastUserCPU)
   {
      //Overflow detection. Just skip this value.
      percent = -1.0;
   }
   else
   {
      percent = (timeSample.tms_stime - lastSysCPU) +
                (timeSample.tms_utime - lastUserCPU);
      percent /= (now - lastCPU);
      percent /= numProcessors;
      percent *= 100;
   }
   lastCPU = now;
   lastSysCPU = timeSample.tms_stime;
   lastUserCPU = timeSample.tms_utime;

   return percent;
}
