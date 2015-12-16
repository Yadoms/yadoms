#include "stdafx.h"
#include "SystemFactory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> context, const std::string & device, const ISIConfiguration& configuration):
   m_PluginName                       (device),
   m_MemoryLoad                       (device),
   m_CPULoad                          (device),
   m_YadomsCPULoad                    (device),
   m_YadomsRAMProcessMemory           (device),
   m_YadomsVirtualProcessMemory       (device)
{
      // Keywords declaration, if needed
      m_MemoryLoad.declareKeywords           (context);
      m_CPULoad.declareKeywords              (context);

      if (configuration.IsAdvancedEnabled())
      {
         m_YadomsCPULoad.declareKeywords              (context);
         m_YadomsRAMProcessMemory.declareKeywords     (context);
         m_YadomsVirtualProcessMemory.declareKeywords (context);
      }

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

void CSystemFactory::OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> context , const ISIConfiguration& configuration)
{
    std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

    YADOMS_LOG(debug) << "Speed reads";

    m_CPULoad.read();

    KeywordList.push_back (m_CPULoad.GetHistorizable());

    if (configuration.IsAdvancedEnabled())
    {
       m_YadomsCPULoad.read();
       KeywordList.push_back (m_YadomsCPULoad.GetHistorizable());
    }

    context->historizeData(m_PluginName, KeywordList);
}
 
void CSystemFactory::OnSlowUpdate ( boost::shared_ptr<yApi::IYPluginApi> context , const ISIConfiguration& configuration)
{
    std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

    YADOMS_LOG(debug) << "Slow reads";

    m_MemoryLoad.read();
    KeywordList.push_back ( m_MemoryLoad.GetHistorizable() );

    if (configuration.IsAdvancedEnabled())
    {
       m_YadomsRAMProcessMemory.read();
       m_YadomsVirtualProcessMemory.read();

       KeywordList.push_back (m_YadomsRAMProcessMemory.GetHistorizable());
       KeywordList.push_back (m_YadomsVirtualProcessMemory.GetHistorizable());
    }

    for(std::vector<boost::shared_ptr<CDiskUsage> >::iterator disksListIterator=m_DiskUsageList.begin(); disksListIterator!=m_DiskUsageList.end(); ++disksListIterator)
    {
        (*disksListIterator)->read();
        KeywordList.push_back ( (*disksListIterator)->GetHistorizable() );
    }

    context->historizeData(m_PluginName, KeywordList);
}

void CSystemFactory::OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> context, const ISIConfiguration& configuration )
{
      if (configuration.IsAdvancedEnabled())
      {
         std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

         m_YadomsCPULoad.declareKeywords              (context);
         m_YadomsRAMProcessMemory.declareKeywords     (context);
         m_YadomsVirtualProcessMemory.declareKeywords (context);

         // We read immediately values to avoid the wait of timers
         m_YadomsCPULoad.read();
         KeywordList.push_back (m_YadomsCPULoad.GetHistorizable());

         m_YadomsRAMProcessMemory.read();
         m_YadomsVirtualProcessMemory.read();

         KeywordList.push_back (m_YadomsRAMProcessMemory.GetHistorizable());
         KeywordList.push_back (m_YadomsVirtualProcessMemory.GetHistorizable());

         context->historizeData(m_PluginName, KeywordList);
      }
}
