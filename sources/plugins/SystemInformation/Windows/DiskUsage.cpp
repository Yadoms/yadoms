#include "stdafx.h"
#include "DiskUsage.h"
#include <shared/exception/Exception.hpp>

CDiskUsage::CDiskUsage(const std::string& device, const std::string& keywordName, const std::string& driveName)
   : m_device(device),
     m_driveName(driveName),
     m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
}

CDiskUsage::~CDiskUsage()
{
}

void CDiskUsage::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
   if (!api->keywordExists(m_device, m_keyword))
      api->declareKeyword(m_device, m_keyword, details);
}

void CDiskUsage::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   api->historizeData(m_device, m_keyword);
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

   m_keyword->set(DiskUsage);

   std::cout << m_driveName << " Disk Usage : " << m_keyword->formatValue() << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CDiskUsage::GetHistorizable() const
{
   return m_keyword;
}

