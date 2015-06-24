#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CMemoryLoad::CMemoryLoad(const std::string & device)
   :m_device(device), m_keyword("MemoryLoad")
{
}

CMemoryLoad::~CMemoryLoad()
{
}

void CMemoryLoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
      if (!context->keywordExists( m_device, m_keyword.getKeyword()))
      {
         context->declareKeyword(m_device, m_keyword);
      }
}

void CMemoryLoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, m_keyword);
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

   m_keyword.set((float(statex.ullTotalPhys - statex.ullAvailPhys)*100 / statex.ullTotalPhys));
   YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Memory Load : " << m_keyword.formatValue();
}


