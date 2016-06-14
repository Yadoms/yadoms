#include "stdafx.h"
#include "YadomsRAMProcessMemory.h"
#include <shared/exception/Exception.hpp>

CYadomsRAMProcessMemory::CYadomsRAMProcessMemory(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CKByte>(keywordName))
{
}

CYadomsRAMProcessMemory::~CYadomsRAMProcessMemory()
{
}

int CYadomsRAMProcessMemory::parseLine(char* line) const
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

   std::cout << "RAM Memory Current Process : " << m_keyword->formatValue() << std::endl;
}