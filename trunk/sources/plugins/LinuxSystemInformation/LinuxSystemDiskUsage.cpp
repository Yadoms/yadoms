#include "stdafx.h"
#include "LinuxSystemDiskUsage.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CLinuxSystemDiskUsage::CLinuxSystemDiskUsage(const std::string & deviceId, const std::string & driveName)
   :m_deviceId(deviceId), m_driveName(driveName)
{
}

CLinuxSystemDiskUsage::~CLinuxSystemDiskUsage()
{}

const std::string& CLinuxSystemDiskUsage::getDeviceId() const
{
   return m_deviceId;
}

void CLinuxSystemDiskUsage::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   //context->declareKeyword(m_deviceId, "LinuxDiskUsage", "DiskUsage", yApi::IYadomsApi::kReadOnly , yApi::IYadomsApi::kDecimal, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CLinuxSystemDiskUsage::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "LinuxDiskUsage"  , m_diskUsage);
}

double CLinuxSystemDiskUsage::getValue()
{


   return 0;
}

/* TODO : TO BE TESTED UNDER WINDOWS AND LINUX !!!!

#include <boost/filesystem.hpp> 
#include <iostream> 

int main() 
{ 
  boost::filesystem::path p("C:\\"); 
  try 
  { 
    boost::filesystem::space_info s = boost::filesystem::space(p); 
    std::cout << s.capacity << std::endl; 
    std::cout << s.free << std::endl; 
    std::cout << s.available << std::endl; 
  } 
  catch (boost::filesystem::filesystem_error &e) 
  { 
    std::cerr << e.what() << std::endl; 
  } 
} 

*/
