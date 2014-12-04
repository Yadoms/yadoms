#include "stdafx.h"
#include <shared/Log.h>
#include "LinuxSystemCPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

CLinuxSystemCPULoad::CLinuxSystemCPULoad(const std::string & device)
   :m_device(device), m_keyword("LinuxCPULoad")
{
   FILE* file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %Lu %Lu %Lu %Lu", &lastTotalUser, &lastTotalUserLow,
      &lastTotalSys, &lastTotalIdle);
   fclose(file);
}

CLinuxSystemCPULoad::~CLinuxSystemCPULoad()
{
}

void CLinuxSystemCPULoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_keyword);
}

void CLinuxSystemCPULoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, m_keyword);
}

void CLinuxSystemCPULoad::read()
{
   //TODO : Keep the last value, if an overflow occured
   double percent;
   unsigned long long totalUser, totalUserLow, totalSys, totalIdle;
   FILE* file;

   file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %Lu %Lu %Lu %Lu", &totalUser, &totalUserLow,
      &totalSys, &totalIdle);
   fclose(file);

   if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
      totalSys < lastTotalSys || totalIdle < lastTotalIdle){
         //Overflow detection. Just skip this value.
         percent = -1.0;
   }
   else{
      unsigned long long total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
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

   m_keyword.set (percent);
   YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  CPU Load : " << m_keyword.formatValue();
}


