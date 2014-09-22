#include "stdafx.h"
#include "WindowsSystemDiskUsage.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CWindowsSystemDiskUsage::CWindowsSystemDiskUsage(const std::string & device, const std::string & keywordName, const std::string & driveName)
   :m_device(device), m_keyword(keywordName), m_driveName(driveName)
{
}

CWindowsSystemDiskUsage::~CWindowsSystemDiskUsage()
{
}

void CWindowsSystemDiskUsage::declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_keyword);
}

void CWindowsSystemDiskUsage::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_device, m_keyword);
}

void CWindowsSystemDiskUsage::read()
{
   ULARGE_INTEGER FreeSpaceAvailable, TotalSpace, TotalFreeSpaceAvailable;

   if (!GetDiskFreeSpaceEx(m_driveName.c_str(), &FreeSpaceAvailable, &TotalSpace, &TotalFreeSpaceAvailable))
   {
      std::stringstream Message;
      Message << "GetDiskFreeSpaceEx return an error : ";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   m_keyword.set((1 - (float) FreeSpaceAvailable.QuadPart / TotalSpace.QuadPart) * 100);
   YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  " << m_driveName << " Disk Usage : " << m_keyword.formatValue();
}


