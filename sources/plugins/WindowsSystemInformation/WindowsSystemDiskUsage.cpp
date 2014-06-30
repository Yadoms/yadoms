#include "stdafx.h"
#include "WindowsSystemDiskUsage.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CWindowsSystemDiskUsage::CWindowsSystemDiskUsage(const std::string & deviceId, const std::string & driveName)
   :m_deviceId(deviceId), m_driveName(driveName)
{
}

CWindowsSystemDiskUsage::~CWindowsSystemDiskUsage()
{}

const std::string& CWindowsSystemDiskUsage::getDeviceId() const
{
   return m_deviceId;
}

void CWindowsSystemDiskUsage::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareCustomKeyword(m_deviceId, "WindowsDiskUsage", "DiskUsage", yApi::kReadOnly, yApi::kDecimal, yApi::CStandardUnits::Percent);
}

void CWindowsSystemDiskUsage::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, "WindowsDiskUsage"  , m_diskUsage);
}

double CWindowsSystemDiskUsage::getValue()
{
   ULARGE_INTEGER FreeSpaceAvailable, TotalSpace, TotalFreeSpaceAvailable;

   if (!GetDiskFreeSpaceEx(m_driveName.c_str(), &FreeSpaceAvailable, &TotalSpace, &TotalFreeSpaceAvailable))
   {
      std::stringstream Message;
      Message << "GetDiskFreeSpaceEx return an error : ";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   m_diskUsage = (1 - (double) FreeSpaceAvailable.QuadPart / TotalSpace.QuadPart) * 100;

   return m_diskUsage;
}


