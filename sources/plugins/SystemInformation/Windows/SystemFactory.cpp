#include "stdafx.h"
#include "SystemFactory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api, const std::string & device, const ISIConfiguration& configuration, shared::CDataContainer details):
   m_PluginName           (device),
   m_MemoryLoad           (device),
   m_CPULoad              (device),
   m_YadomsCPULoad        (device),
   m_RAMProcessMemory     (device),
   m_VirtualProcessMemory (device)
{
      // Keywords declaration, if needed
      m_MemoryLoad.declareKeywords           (api, details);
      m_CPULoad.declareKeywords              (api, details);
      m_YadomsCPULoad.declareKeywords        (api, details);

      if (configuration.IsAdvancedEnabled())
      {
         m_RAMProcessMemory.declareKeywords     (api, details);
         m_VirtualProcessMemory.declareKeywords (api, details);
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
         DiskUsage.declareKeywords(api, details);
      }
}

CSystemFactory::~CSystemFactory()
{
}

void CSystemFactory::OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> api )
{
    std::cout << "Speed Updates" << std::endl;

    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > KeywordList;

    m_CPULoad.read();
    m_YadomsCPULoad.read();

    KeywordList.push_back (m_CPULoad.GetHistorizable());
    KeywordList.push_back (m_YadomsCPULoad.GetHistorizable());

    api->historizeData(m_PluginName, KeywordList);
}
 
void CSystemFactory::OnSlowUpdate ( boost::shared_ptr<yApi::IYPluginApi> api , const ISIConfiguration& configuration)
{
    std::cout << "Slow Updates" << std::endl;

    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > KeywordList;

    m_MemoryLoad.read();
    m_RAMProcessMemory.read();
    m_VirtualProcessMemory.read();

    KeywordList.push_back ( m_MemoryLoad.GetHistorizable() );

    if (configuration.IsAdvancedEnabled())
    {
       KeywordList.push_back (m_RAMProcessMemory.GetHistorizable());
       KeywordList.push_back (m_VirtualProcessMemory.GetHistorizable());
    }

    for(std::vector<CDiskUsage>::iterator disksListIterator=DiskUsageList.begin(); disksListIterator!=DiskUsageList.end(); ++disksListIterator)
    {
        disksListIterator->read();
	    KeywordList.push_back ( disksListIterator->GetHistorizable() );
    }

    api->historizeData(m_PluginName, KeywordList);
}

void CSystemFactory::OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> api, const ISIConfiguration& configuration, shared::CDataContainer details )
{
      if (configuration.IsAdvancedEnabled())
      {
		 std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > KeywordList;

         m_RAMProcessMemory.declareKeywords     (api, details);
         m_VirtualProcessMemory.declareKeywords (api, details);

         // We read immediately values to avoid the wait of timers

         m_RAMProcessMemory.read();
         m_VirtualProcessMemory.read();

         KeywordList.push_back (m_RAMProcessMemory.GetHistorizable());
         KeywordList.push_back (m_VirtualProcessMemory.GetHistorizable());

         api->historizeData(m_PluginName, KeywordList);
      }
}