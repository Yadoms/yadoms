#include "stdafx.h"
#include "DiskUsage.h"
#include <shared/exception/Exception.hpp>
#include "Helpers.h"

CDiskUsage::CDiskUsage(const std::string& keywordName,
                       const std::string& driveName)
   : m_driveName(driveName),
     m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
}

CDiskUsage::~CDiskUsage()
{
}

void CDiskUsage::read()
{
   ULARGE_INTEGER FreeSpaceAvailable, TotalSpace, TotalFreeSpaceAvailable;

   if (!GetDiskFreeSpaceEx(m_driveName.c_str(), &FreeSpaceAvailable, &TotalSpace, &TotalFreeSpaceAvailable))
   {
      std::stringstream Message;
      Message << "GetDiskFreeSpaceEx return an error : ";
      Message << GetLastError();
      throw shared::exception::CException(Message.str());
   }

   float DiskUsage = static_cast<float>(floor((1 - static_cast<float>(FreeSpaceAvailable.QuadPart) / TotalSpace.QuadPart) * 1000)) / 10;

   m_keyword->set(valueRoundWithPrecision(DiskUsage,3));
}