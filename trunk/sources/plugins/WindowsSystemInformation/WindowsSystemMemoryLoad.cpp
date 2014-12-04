#include "stdafx.h"
#include "WindowsSystemMemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CWindowsSystemMemoryLoad::CWindowsSystemMemoryLoad(const std::string & device)
   :m_device(device), m_keyword("MemoryLoad")
{
}

CWindowsSystemMemoryLoad::~CWindowsSystemMemoryLoad()
{
}

void CWindowsSystemMemoryLoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   context->declareKeyword(m_device, m_keyword);
}

void CWindowsSystemMemoryLoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, m_keyword);
}

void CWindowsSystemMemoryLoad::read()
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


