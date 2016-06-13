#include "stdafx.h"
#include "SystemFactory.h"
#include "DisksList.h"

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& device,
                               const ISIConfiguration& configuration,
                               shared::CDataContainer details)
   : m_DeviceName(device),
     m_MemoryLoad(boost::make_shared<CMemoryLoad>("MemoryLoad")),
     m_CPULoad(boost::make_shared<CCPULoad>("CPULoad")),
     m_YadomsCPULoad(boost::make_shared<CYadomsCPULoad>("YadomsCPULoad")),
     m_TemperatureSensor(boost::make_shared<CTemperatureSensor>("Temperature")),
     m_highFreqencyUpdateKeywords({ m_CPULoad->historizable(), m_YadomsCPULoad->historizable(), m_TemperatureSensor->historizable() }),
     m_lowFreqencyUpdateKeywords({ m_MemoryLoad->historizable() })
{
   if (configuration.IsAdvancedEnabled())
   {
      m_RAMProcessMemory = boost::make_shared<CRAMProcessMemory>("YadomsRAMProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_RAMProcessMemory->historizable());
      m_VirtualProcessMemory = boost::make_shared<CVirtualProcessMemory>("YadomsVirtualProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_VirtualProcessMemory->historizable());
   }

   // As disk list can change (add a disk), update it each time Yadoms starts

   // Disk usage for all disks
   auto diskList = CDisksList().getList();
   for (auto disk = diskList.begin(); disk != diskList.end(); ++disk)
   {
      auto diskUsage = boost::make_shared<CDiskUsage>(disk->substr(5, 4) + "_DiskUsage", *disk);
      m_diskUsageList.push_back(diskUsage);
      m_lowFreqencyUpdateKeywords.push_back(diskUsage->historizable());
   }

   api->declareDevice(device, Model, m_highFreqencyUpdateKeywords, details);
   api->declareDevice(device, Model, m_lowFreqencyUpdateKeywords, details);
}

CSystemFactory::~CSystemFactory()
{
}

void CSystemFactory::OnHighFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   std::cout << "High frequency updates" << std::endl;

   m_CPULoad->read();
   m_YadomsCPULoad->read();
   m_TemperatureSensor->read();

   api->historizeData(m_DeviceName, m_highFreqencyUpdateKeywords);
}

void CSystemFactory::OnLowFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const ISIConfiguration& configuration)
{
   std::cout << "Low frequency updates" << std::endl;

   m_MemoryLoad->read();

   if (configuration.IsAdvancedEnabled())
   {
      m_RAMProcessMemory->read();
      m_VirtualProcessMemory->read();
   }

   for (auto disk = m_diskUsageList.begin(); disk != m_diskUsageList.end(); ++disk)
      (*disk)->read();

   api->historizeData(m_DeviceName, m_lowFreqencyUpdateKeywords);
}

void CSystemFactory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const ISIConfiguration& configuration,
                                           shared::CDataContainer details)
{
   if (configuration.IsAdvancedEnabled())
   {
      m_RAMProcessMemory = boost::make_shared<CRAMProcessMemory>("YadomsRAMProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_RAMProcessMemory->historizable());
      m_VirtualProcessMemory = boost::make_shared<CVirtualProcessMemory>("YadomsVirtualProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_VirtualProcessMemory->historizable());
      api->declareDevice(m_DeviceName, Model, m_lowFreqencyUpdateKeywords, details);

      // We read immediately values to avoid the wait of timers

      m_RAMProcessMemory->read();
      m_VirtualProcessMemory->read();

      api->historizeData(m_DeviceName, m_RAMProcessMemory->historizable());
      api->historizeData(m_DeviceName, m_VirtualProcessMemory->historizable());
   }
   else
   {
      // Local removing of existing keywords
      if (!!m_RAMProcessMemory)
      {
         auto it = std::find(m_lowFreqencyUpdateKeywords.begin(), m_lowFreqencyUpdateKeywords.end(), m_RAMProcessMemory->historizable());
         if (it != m_lowFreqencyUpdateKeywords.end())
            m_lowFreqencyUpdateKeywords.erase(it);
         m_RAMProcessMemory.reset();
      }
      if (!!m_VirtualProcessMemory)
      {
         auto it = std::find(m_lowFreqencyUpdateKeywords.begin(), m_lowFreqencyUpdateKeywords.end(), m_VirtualProcessMemory->historizable());
         if (it != m_lowFreqencyUpdateKeywords.end())
            m_lowFreqencyUpdateKeywords.erase(it);
         m_VirtualProcessMemory.reset();
      }
   }
}

