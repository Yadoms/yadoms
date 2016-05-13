#include "stdafx.h"
#include "YadomsVirtualProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

CYadomsVirtualProcessMemory::CYadomsVirtualProcessMemory(const std::string & device)
   :m_device(device), 
    m_keyword(new yApi::historization::CKByte("YadomsVirtualMemory") )
{}

CYadomsVirtualProcessMemory::~CYadomsVirtualProcessMemory()
{}

void CYadomsVirtualProcessMemory::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
      if (!api->keywordExists( m_device, m_keyword->getKeyword()))
         api->declareKeyword(m_device, *m_keyword, details);
}

void CYadomsVirtualProcessMemory::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   BOOST_ASSERT_MSG(!!api, "api must be defined");

   api->historizeData(m_device, *m_keyword);
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

   std::cout << "Virtual Memory for Current Process : " << m_keyword->formatValue() << std::endl;
}

boost::shared_ptr<yApi::historization::IHistorizable> CYadomsVirtualProcessMemory::GetHistorizable() const
{
	return m_keyword;
}
