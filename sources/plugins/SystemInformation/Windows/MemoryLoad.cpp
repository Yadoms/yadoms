#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>

CMemoryLoad::CMemoryLoad(const std::string & device)
   :m_device(device), 
    m_keyword(boost::make_shared< yApi::historization::CLoad>("MemoryLoad"))
{}

CMemoryLoad::~CMemoryLoad()
{}

void CMemoryLoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
      if (!api->keywordExists( m_device, m_keyword))
         api->declareKeyword(m_device, m_keyword, details);
}

void CMemoryLoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   api->historizeData(m_device, m_keyword);
}

void CMemoryLoad::read()
{
   MEMORYSTATUSEX statex;

   statex.dwLength = sizeof (statex);

   if (!GlobalMemoryStatusEx (&statex))
   {
      std::stringstream Message;
      Message << "Fail to read Windows system memory size :";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }

   float MemoryLoad = (float) floor((float(statex.ullTotalPhys - statex.ullAvailPhys)*100 / statex.ullTotalPhys)*10) /10;

   m_keyword->set( MemoryLoad );

   std::cout << "Memory Load : " << m_keyword->formatValue() << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CMemoryLoad::GetHistorizable() const
{
	return m_keyword;
}