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
     m_highFreqencyUpdateKeywords({ m_cpuLoad->historizable() }),
     m_lowFreqencyUpdateKeywords({ m_memoryLoad->historizable() })
{
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

   api->historizeData(m_deviceName, m_highFreqencyUpdateKeywords);
}

void CSystemFactory::OnLowFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                          const ISIConfiguration& configuration)
{
   std::cout << "Low frequency updates" << std::endl;

   m_memoryLoad->read();

   for (auto disk = m_diskUsageList.begin(); disk != m_diskUsageList.end(); ++disk)
      (*disk)->read();

   api->historizeData(m_deviceName, m_lowFreqencyUpdateKeywords);
}

void CSystemFactory::OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const ISIConfiguration& configuration,
                                           shared::CDataContainer details)
{
}

