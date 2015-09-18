#include "stdafx.h"
#include "VirtualProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>
#include "windows.h"
#include "psapi.h"

CVirtualProcessMemory::CVirtualProcessMemory(const std::string & device)
   :m_device(device), 
    m_keyword(new yApi::historization::CKByte("VirtualProcessMemory") )
{}

CVirtualProcessMemory::~CVirtualProcessMemory()
{}

void CVirtualProcessMemory::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
      if (!context->keywordExists( m_device, m_keyword->getKeyword()))
      {
         context->declareKeyword(m_device, *m_keyword);
      }
}

void CVirtualProcessMemory::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, *m_keyword);
}

void CVirtualProcessMemory::read()
{
   PROCESS_MEMORY_COUNTERS_EX pmc;

   if (!GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
   {
      std::stringstream Message;
      Message << "Fail to read Windows virtual process memory size :";
      Message << GetLastError();
      throw shared::exception::CException ( Message.str() );
   }
   
   long VirtualProcessMemory = pmc.PrivateUsage / 1000;

   if (m_keyword->get() != VirtualProcessMemory )
      m_keyword->set( VirtualProcessMemory );

   YADOMS_LOG(debug) << "Virtual Memory for Current Process : " << m_keyword->formatValue();
}

boost::shared_ptr<yApi::historization::IHistorizable> CVirtualProcessMemory::GetHistorizable() const
{
	return m_keyword;
}
