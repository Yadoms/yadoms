#include "stdafx.h"
#include "YadomsCPULoad.h"
#include <shared/exception/Exception.hpp>

CYadomsCPULoad::CYadomsCPULoad(const std::string& keywordName)
   :m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
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

CYadomsCPULoad::~CYadomsCPULoad()
{
}

void CYadomsCPULoad::read()
{
   struct tms timeSample;
   clock_t now;
   float percent;


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

   m_keyword->set( percent );

   std::cout << "Yadoms CPU Load : " << m_keyword->formatValue() << std::endl;
}
