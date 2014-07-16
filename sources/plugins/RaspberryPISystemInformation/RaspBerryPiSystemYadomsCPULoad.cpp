#include "stdafx.h"
#include "RaspBerryPiSystemYadomsCPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CRaspBerryPiSystemYadomsCPULoad::CRaspBerryPiSystemYadomsCPULoad(const std::string & deviceId)
   :m_deviceId(deviceId), m_CPULoad(0), m_Keyword("YadomsCPULoad"), m_Capacity("cpuload")
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

CRaspBerryPiSystemYadomsCPULoad::~CRaspBerryPiSystemYadomsCPULoad()
{}

const std::string& CRaspBerryPiSystemYadomsCPULoad::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CRaspBerryPiSystemYadomsCPULoad::getCapacity() const
{
   return m_Capacity;
}

const std::string& CRaspBerryPiSystemYadomsCPULoad::getKeyword() const
{
   return m_Keyword;
}

void CRaspBerryPiSystemYadomsCPULoad::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareCustomKeyword(m_deviceId, getKeyword()  , getCapacity() , yApi::kGet , yApi::kNumeric, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CRaspBerryPiSystemYadomsCPULoad::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, getKeyword()  , m_CPULoad);
}

double CRaspBerryPiSystemYadomsCPULoad::getValue()
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

   m_CPULoad = percent;

   return percent;
}
