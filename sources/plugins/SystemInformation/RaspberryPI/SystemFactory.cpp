#include "stdafx.h"
#include "SystemFactory.h"
#include "DisksList.h"


static const std::string Model("SystemInformation");

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& device,
                               const ISIConfiguration& configuration,
                               shared::CDataContainer details)
   : m_DeviceName(device),
     m_memoryLoad(boost::make_shared<CMemoryLoad>("MemoryLoad")),
     m_cpuLoad(boost::make_shared<CCPULoad>("CPULoad")),
     m_yadomsCpuLoad(boost::make_shared<CYadomsCPULoad>("YadomsCPULoad")),
     m_temperatureSensor(boost::make_shared<CTemperatureSensor>("Temperature")),
     m_highFreqencyUpdateKeywords({ m_cpuLoad->historizable(), m_yadomsCpuLoad->historizable(), m_temperatureSensor->historizable() }),
     m_lowFreqencyUpdateKeywords({ m_memoryLoad->historizable() })
{
   if (configuration.IsAdvancedEnabled())
   {
      m_yadomsRamProcessMemory = boost::make_shared<CYadomsRAMProcessMemory>("YadomsRAMProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_yadomsRamProcessMemory->historizable());
      m_yadomsVirtualProcessMemory = boost::make_shared<CYadomsVirtualProcessMemory>("YadomsVirtualProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_yadomsVirtualProcessMemory->historizable());
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

   m_cpuLoad->read();
   m_yadomsCpuLoad->read();
   m_temperatureSensor->read();

   api->historizeData(m_DeviceName, m_highFreqencyUpdateKeywords);
}

void CSystemFactory::OnLowFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const ISIConfiguration& configuration)
{
   std::cout << "Low frequency updates" << std::endl;

   m_memoryLoad->read();

   if (configuration.IsAdvancedEnabled())
   {
      m_yadomsRamProcessMemory->read();
      m_yadomsVirtualProcessMemory->read();
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
      m_yadomsRamProcessMemory = boost::make_shared<CRAMProcessMemory>("YadomsRAMProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_yadomsRamProcessMemory->historizable());
      m_yadomsVirtualProcessMemory = boost::make_shared<CVirtualProcessMemory>("YadomsVirtualProcessMemory");
      m_lowFreqencyUpdateKeywords.push_back(m_yadomsVirtualProcessMemory->historizable());
      api->declareDevice(m_DeviceName, Model, m_lowFreqencyUpdateKeywords, details);

      // We read immediately values to avoid the wait of timers

      m_yadomsRamProcessMemory->read();
      m_yadomsVirtualProcessMemory->read();

      api->historizeData(m_DeviceName, m_yadomsRamProcessMemory->historizable());
      api->historizeData(m_DeviceName, m_yadomsVirtualProcessMemory->historizable());
   }
   else
   {
      // Local removing of existing keywords
      if (!!m_yadomsRamProcessMemory)
      {
         auto it = std::find(m_lowFreqencyUpdateKeywords.begin(), m_lowFreqencyUpdateKeywords.end(), m_yadomsRamProcessMemory->historizable());
         if (it != m_lowFreqencyUpdateKeywords.end())
            m_lowFreqencyUpdateKeywords.erase(it);
         m_yadomsRamProcessMemory.reset();
      }
      if (!!m_yadomsVirtualProcessMemory)
      {
         auto it = std::find(m_lowFreqencyUpdateKeywords.begin(), m_lowFreqencyUpdateKeywords.end(), m_yadomsVirtualProcessMemory->historizable());
         if (it != m_lowFreqencyUpdateKeywords.end())
            m_lowFreqencyUpdateKeywords.erase(it);
         m_yadomsVirtualProcessMemory.reset();
      }
   }
}

