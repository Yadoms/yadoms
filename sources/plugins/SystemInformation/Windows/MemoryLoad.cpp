#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CMemoryLoad::CMemoryLoad(const std::string & device)
   :m_device(device), 
    m_keyword( new yApi::historization::CLoad("MemoryLoad"))
{}

CMemoryLoad::~CMemoryLoad()
{}

void CMemoryLoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
      if (!context->keywordExists( m_device, m_keyword->getKeyword()))
      {
         context->declareKeyword(m_device, *m_keyword);
      }
}

void CMemoryLoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, *m_keyword);
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

   if (m_keyword->get() != MemoryLoad )
      m_keyword->set( MemoryLoad );

   YADOMS_LOG(debug) << "Memory Load : " << m_keyword->formatValue();
}

boost::shared_ptr<yApi::historization::IHistorizable> CMemoryLoad::GetHistorizable() const
{
	return m_keyword;
}