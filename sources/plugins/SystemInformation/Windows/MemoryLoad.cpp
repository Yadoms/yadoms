#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include "Helpers.h"

CMemoryLoad::CMemoryLoad(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
}

CMemoryLoad::~CMemoryLoad()
{
}

void CMemoryLoad::read()
{
   MEMORYSTATUSEX statex;

   statex.dwLength = sizeof (statex);

   if (!GlobalMemoryStatusEx(&statex))
   {
      std::stringstream Message;
      Message << "Fail to read Windows system memory size :";
      Message << GetLastError();
      throw shared::exception::CException(Message.str());
   }

   float MemoryLoad = static_cast<float>(floor(float(statex.ullTotalPhys - statex.ullAvailPhys) * 100 / statex.ullTotalPhys * 10)) / 10;

   m_keyword->set(valueRoundWithPrecision(MemoryLoad,3));
}