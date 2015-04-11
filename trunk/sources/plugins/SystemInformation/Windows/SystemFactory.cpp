#include "stdafx.h"
#include "SystemFactory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> context, const std::string & device, const ISIConfiguration& configuration):
   m_PluginName           (device),
   m_MemoryLoad           (device),
   m_CPULoad              (device),
   m_YadomsCPULoad        (device),
   m_RAMProcessMemory     (device),
   m_VirtualProcessMemory (device)
{
      // Keywords declaration, if needed
      m_MemoryLoad.declareKeywords           (context);
      m_CPULoad.declareKeywords              (context);
      m_YadomsCPULoad.declareKeywords        (context);

      if (configuration.IsAdvancedEnabled())
      {
         m_RAMProcessMemory.declareKeywords     (context);
         m_VirtualProcessMemory.declareKeywords (context);
      }

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

void CSystemFactory::OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> context , const ISIConfiguration& configuration)
{
    YADOMS_LOG(debug) << "Speed Updates";

    std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

    m_CPULoad.read();
    m_YadomsCPULoad.read();
    m_RAMProcessMemory.read();
    m_VirtualProcessMemory.read();

    KeywordList.push_back (m_CPULoad.GetHistorizable());
    KeywordList.push_back (m_YadomsCPULoad.GetHistorizable());

    if (configuration.IsAdvancedEnabled())
    {
       KeywordList.push_back (m_RAMProcessMemory.GetHistorizable());
       KeywordList.push_back (m_VirtualProcessMemory.GetHistorizable());
    }

    context->historizeData(m_PluginName, KeywordList);
}
 
void CSystemFactory::OnSlowUpdate ( boost::shared_ptr<yApi::IYPluginApi> context , const ISIConfiguration& configuration)
{
    YADOMS_LOG(debug) << "Slow Updates";

    std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

    m_MemoryLoad.read();

    KeywordList.push_back ( m_MemoryLoad.GetHistorizable() );

    for(std::vector<CDiskUsage>::iterator disksListIterator=DiskUsageList.begin(); disksListIterator!=DiskUsageList.end(); ++disksListIterator)
    {
        disksListIterator->read();
	KeywordList.push_back ( disksListIterator->GetHistorizable() );
    }

    context->historizeData(m_PluginName, KeywordList);
}

void CSystemFactory::OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> context, const ISIConfiguration& configuration )
{
      if (configuration.IsAdvancedEnabled())
      {
         m_RAMProcessMemory.declareKeywords     (context);
         m_VirtualProcessMemory.declareKeywords (context);
      }
}
