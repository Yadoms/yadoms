#include "stdafx.h"
#include "SystemFactory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> context, const std::string & device):
   m_MemoryLoad    (device),
   m_CPULoad       (device),
   m_YadomsCPULoad (device)
{
      // Keywords declaration, if needed
      m_MemoryLoad.declareKeywords(context);
      m_CPULoad.declareKeywords(context);
      m_YadomsCPULoad.declareKeywords(context);

	  // As disk list can change (add a disk), update it each time Yadoms starts

      // Disk usage for all disks
      CDisksList DisksList;
      std::vector<std::string> TempList = DisksList.getList();

      for(std::vector<std::string>::const_iterator disksListIterator = TempList.begin(); disksListIterator != TempList.end(); ++ disksListIterator)
      {
         std::string diskKeywordName = disksListIterator->substr(0, 1) + "_DiskUsage";
         CDiskUsage DiskUsage(device, diskKeywordName, *disksListIterator);
         DiskUsageList.push_back(DiskUsage);
         if (!context->keywordExists(device, diskKeywordName))
            DiskUsage.declareKeywords(context);
      }
}

CSystemFactory::~CSystemFactory()
{
}

void CSystemFactory::OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> context )
{
    YADOMS_LOG(debug) << "WindowsSystem plugin :  Read CPU Loads";

    m_CPULoad.read();
    m_YadomsCPULoad.read();

    m_CPULoad.historizeData(context);
    m_YadomsCPULoad.historizeData(context);
}
 
void CSystemFactory::OnSlowUpdate ( boost::shared_ptr<yApi::IYPluginApi> context )
{
	YADOMS_LOG(debug) << "WindowsSystem plugin :  Read Memory and disk Usages";

    m_MemoryLoad.read();
    m_MemoryLoad.historizeData(context);

    for(std::vector<CDiskUsage>::iterator disksListIterator=DiskUsageList.begin(); disksListIterator!=DiskUsageList.end(); ++disksListIterator)
    {
        disksListIterator->read();
        disksListIterator->historizeData(context);
    }
}

