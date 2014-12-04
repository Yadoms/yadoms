#include "stdafx.h"
#include <shared/Log.h>
#include "RaspBerryPiSystemCPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

CRaspBerryPiSystemCPULoad::CRaspBerryPiSystemCPULoad(const std::string & device)
   :m_device(device), m_keyword("RaspBerryPiCPULoad")
{
   FILE* file = fopen("/proc/stat", "r");
   fscanf(file, "cpu %Lu %Lu %Lu %Lu", &lastTotalUser, &lastTotalUserLow,
      &lastTotalSys, &lastTotalIdle);
   fclose(file);
}

CRaspBerryPiSystemCPULoad::~CRaspBerryPiSystemCPULoad()
{
}

void CRaspBerryPiSystemCPULoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_keyword);
}

void CRaspBerryPiSystemCPULoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, m_keyword);
}

void CRaspBerryPiSystemCPULoad::read()
{
   //TODO : Keep the last value, if an overflow occured
   float percent;
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
   YADOMS_LOG(debug) << "RaspBerryPiSystemInformation plugin :  CPU Load : " << m_keyword.formatValue();
}


