#include "stdafx.h"
#include "SystemFactory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string & device,
                               const ISIConfiguration& configuration,
                               shared::CDataContainer details):
   m_PluginName                       (device),
   m_MemoryLoad                       (device),
   m_CPULoad                          (device),
   m_YadomsCPULoad                    (device),
   m_YadomsRAMProcessMemory           (device),
   m_YadomsVirtualProcessMemory       (device)
{
   m_MemoryLoad.declareKeywords           (api, details);
   m_CPULoad.declareKeywords              (api, details);
   m_YadomsCPULoad.declareKeywords        (api, details);

   if (configuration.IsAdvancedEnabled())
   {
      m_YadomsRAMProcessMemory.declareKeywords     (api, details);
      m_YadomsVirtualProcessMemory.declareKeywords (api, details);
   }

   // As disk list can change (add a disk), update it each time Yadoms starts

   // Disk usage for all disks
   CDisksList DisksList;
   std::vector<std::string> TempList = DisksList.getList();

   for(std::vector<std::string>::const_iterator disksListIterator = TempList.begin(); disksListIterator != TempList.end(); ++ disksListIterator)
   {
      std::string diskKeywordName = disksListIterator->substr(5, 4) + "_DiskUsage";
      auto DiskUsage = boost::make_shared<CDiskUsage>(device, *disksListIterator, diskKeywordName);
      m_DiskUsageList.push_back(DiskUsage);
      DiskUsage->declareKeywords(api, details);
   }
}

CSystemFactory::~CSystemFactory()
{
}

void CSystemFactory::OnSpeedUpdate ( boost::shared_ptr<yApi::IYPluginApi> api )
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > KeywordList;

   std::cout << "Speed reads" << std::endl;

   m_CPULoad.read();
   m_YadomsCPULoad.read();

   KeywordList.push_back (m_CPULoad.GetHistorizable());
   KeywordList.push_back (m_YadomsCPULoad.GetHistorizable());

   api->historizeData(m_PluginName, KeywordList);
}

void CSystemFactory::OnSlowUpdate ( boost::shared_ptr<yApi::IYPluginApi> api , const ISIConfiguration& configuration)
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > KeywordList;

   std::cout << "Slow reads" << std::endl;

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

   api->historizeData(m_PluginName, KeywordList);
}

void CSystemFactory::OnConfigurationUpdate ( boost::shared_ptr<yApi::IYPluginApi> api, const ISIConfiguration& configuration, shared::CDataContainer details )
{
   if (configuration.IsAdvancedEnabled())
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > KeywordList;

      m_YadomsRAMProcessMemory.declareKeywords     (api, details);
      m_YadomsVirtualProcessMemory.declareKeywords (api, details);

      m_YadomsRAMProcessMemory.read();
      m_YadomsVirtualProcessMemory.read();

      KeywordList.push_back (m_YadomsRAMProcessMemory.GetHistorizable());
      KeywordList.push_back (m_YadomsVirtualProcessMemory.GetHistorizable());

      api->historizeData(m_PluginName, KeywordList);
   }
}
