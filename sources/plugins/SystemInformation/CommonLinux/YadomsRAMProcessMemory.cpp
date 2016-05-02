#include "stdafx.h"
#include "YadomsRAMProcessMemory.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <shared/Log.h>

CYadomsRAMProcessMemory::CYadomsRAMProcessMemory(const std::string & device)
   :m_device(device), 
    m_keyword(new yApi::historization::CKByte("YadomsRAMMemory"))
{}

CYadomsRAMProcessMemory::~CYadomsRAMProcessMemory()
{}

void CYadomsRAMProcessMemory::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context, shared::CDataContainer details)
{
      if (!context->keywordExists( m_device, m_keyword->getKeyword()))
         context->declareKeyword(m_device, *m_keyword, details);
}

void CYadomsRAMProcessMemory::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, *m_keyword);
}

int CYadomsRAMProcessMemory::parseLine(char* line)
{
   int i = strlen(line);
   while (*line < '0' || *line > '9') line++;
   line[i-3] = '\0';
   i = atoi(line);
   return i;
}

void CYadomsRAMProcessMemory::read()
{
   FILE* file = fopen("/proc/self/status", "r");
   int result = -1;
   char line[128];
    
    while (fgets(line, 128, file) != NULL)
    {
       if (strncmp(line, "VmRSS:", 6) == 0)
       {
          result = parseLine(line);
          break;
       }
    }
    fclose(file);

    //Note: this value is in KB!
    m_keyword->set( result );

   YADOMS_LOG(debug) << "RAM Memory Current Process : " << m_keyword->formatValue();
}

boost::shared_ptr<yApi::historization::IHistorizable> CYadomsRAMProcessMemory::GetHistorizable() const
{
	return m_keyword;
}
