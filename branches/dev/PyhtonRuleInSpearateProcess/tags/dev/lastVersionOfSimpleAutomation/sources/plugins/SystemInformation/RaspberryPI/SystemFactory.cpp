#include "stdafx.h"
#include "SystemFactory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> context, const std::string & device):
   m_MemoryLoad        (device),
   m_CPULoad           (device),
   m_YadomsCPULoad     (device),
   m_TemperatureSensor (device)
{
      // Keywords declaration, if needed
      m_MemoryLoad.declareKeywords(context);
      m_CPULoad.declareKeywords(context);
      m_YadomsCPULoad.declareKeywords(context);
      m_TemperatureSensor.declareKeywords(context);

      // As disk list can change (add a disk), update it each time Yadoms starts

      // Disk usage for all disks
      CDisksList DisksList;
      std::vector<std::string> TempList = DisksList.getList();

      for(std::vector<std::string>::const_iterator disksListIterator = TempList.begin(); disksListIterator != TempList.end(); ++ disksListIterator)
      {
         std::string diskKeywordName = disksListIterator->substr(5, 4) + "_DiskUsage";
         boost::shared_ptr<CDiskUsage> DiskUsage;
         DiskUsage.reset (new CDiskUsage( device, *disksListIterator, diskKeywordName ));
         m_DiskUsageList.push_back(DiskUsage);
         if (!context->keywordExists(device, diskKeywordName))
            DiskUsage->declareKeywords(context);
      }
}

CSystemFactory::~CSystemFactory()
{}

void CSystemFactory::OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> context )
{
    YADOMS_LOG(debug) << "System plugin :  Read CPU Loads";

    m_CPULoad.read();
    m_YadomsCPULoad.read();

    m_CPULoad.historizeData(context);
    m_YadomsCPULoad.historizeData(context);
}
 
void CSystemFactory::OnSlowUpdate ( boost::shared_ptr<yApi::IYPluginApi> context )
{
	YADOMS_LOG(debug) << "System plugin :  Read Memory and disk Usages";

    m_MemoryLoad.read();
    m_MemoryLoad.historizeData(context);

    m_TemperatureSensor.read();
    m_TemperatureSensor.historizeData(context);

    for(std::vector<boost::shared_ptr<CDiskUsage> >::iterator disksListIterator=m_DiskUsageList.begin(); disksListIterator!=m_DiskUsageList.end(); ++disksListIterator)
    {
        (*disksListIterator)->read();
        (*disksListIterator)->historizeData(context);
    }
}

