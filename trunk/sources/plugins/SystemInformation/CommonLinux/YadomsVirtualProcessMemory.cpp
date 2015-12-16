#include "stdafx.h"
#include "YadomsVirtualProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CYadomsVirtualProcessMemory::CYadomsVirtualProcessMemory(const std::string & device)
   :m_device(device), 
    m_keyword(new yApi::historization::CKByte("YadomsVirtualMemory") )
{}

CYadomsVirtualProcessMemory::~CYadomsVirtualProcessMemory()
{}

void CYadomsVirtualProcessMemory::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
      if (!context->keywordExists( m_device, m_keyword->getKeyword()))
      {
         context->declareKeyword(m_device, *m_keyword);
      }
}

void CYadomsVirtualProcessMemory::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, *m_keyword);
}

int CYadomsVirtualProcessMemory::parseLine(char* line)
{
   int i = strlen(line);
   while (*line < '0' || *line > '9') line++;
   line[i-3] = '\0';
   i = atoi(line);
   return i;
}

void CYadomsVirtualProcessMemory::read()
{
   FILE* file = fopen("/proc/self/status", "r");
   int result = -1;
   char line[128];


   while (fgets(line, 128, file) != NULL)
   {
      if (strncmp(line, "VmSize:", 7) == 0)
      {
	 result = parseLine(line);
	 break;
      }
    }
    fclose(file);

   //Note: this value is in KB!
   m_keyword->set( result );

   YADOMS_LOG(debug) << "Virtual Memory for Current Process : " << m_keyword->formatValue();
}

boost::shared_ptr<yApi::historization::IHistorizable> CYadomsVirtualProcessMemory::GetHistorizable() const
{
	return m_keyword;
}
