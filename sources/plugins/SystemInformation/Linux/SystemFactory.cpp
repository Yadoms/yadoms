#include "stdafx.h"
#include "SystemFactory.h"
#include "DisksList.h"


static const std::string Model("SystemInformation");

CSystemFactory::CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                               const std::string& device,
                               const ISIConfiguration& configuration,
                               shared::CDataContainer details)
   : m_deviceName(device),
     m_memoryLoad(boost::make_shared<CMemoryLoad>("MemoryLoad")),
     m_cpuLoad(boost::make_shared<CCPULoad>("CPULoad")),
     m_highFrequencyUpdateKeywords({ m_cpuLoad->historizable() }),
     m_lowFrequencyUpdateKeywords({ m_memoryLoad->historizable() })
{
   // As disk list can change (add a disk), update it each time Yadoms starts

   // Disk usage for all disks
   auto diskList = CDisksList().getList();
   for (auto disk = diskList.begin(); disk != diskList.end(); ++disk)
   {
      auto diskUsage = boost::make_shared<CDiskUsage>(disk->substr(5, 4) + "_DiskUsage", *disk);
      m_diskUsageList.push_back(diskUsage);
      m_lowFrequencyUpdateKeywords.push_back(diskUsage->historizable());
   }

   api->declareDevice(device, Model, Model, m_highFrequencyUpdateKeywords, details);
   api->declareDevice(device, Model, Model, m_lowFrequencyUpdateKeywords, details);
}

CSystemFactory::~CSystemFactory()
{
}

void CSystemFactory::OnHighFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   m_cpuLoad->read();
   api->historizeData(m_deviceName, m_highFrequencyUpdateKeywords);
}

void CSystemFactory::OnLowFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const ISIConfiguration& configuration)
{
   m_memoryLoad->read();

   for (auto disk = m_diskUsageList.begin(); disk != m_diskUsageList.end(); ++disk)
      (*disk)->read();

   api->historizeData(m_deviceName, m_lowFrequencyUpdateKeywords);
}

void CSystemFactory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const ISIConfiguration& configuration,
                                           shared::CDataContainer details)
{}

