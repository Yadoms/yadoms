#include "stdafx.h"
#include "MemoryLoad.h"
#include <shared/exception/Exception.hpp>
#include <sys/sysinfo.h>
#include <sys/types.h>

CMemoryLoad::CMemoryLoad(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
}

CMemoryLoad::~CMemoryLoad()
{
}

void CMemoryLoad::ReadFromFile( unsigned long long *dmemTotal,
                                unsigned long long *dmemFree,
                                unsigned long long *dbuffer,
                                unsigned long long *dcached
                              )
{
   std::ifstream procFile("/proc/meminfo");
   
   // 1 : mem Total.
   // 2 : mem free.
   // 3 : buffer.
   // 4 : cached.
   
   boost::regex reg("^(.*): *(\\d+)");
   std::string line;
   unsigned long long memTab[NB_LINE_TO_BE_READ];
   unsigned char counter = 0;

   while ( std::getline(procFile, line) && (counter < NB_LINE_TO_BE_READ) ) 
   {
      std::cout << "line :" << line << std::endl;
      boost::smatch match;
      if ( boost::regex_search( line, match, reg ) ) 
      {
          memTab[ counter ]  = boost::lexical_cast<long long>(match[2]);
      }
      counter++;
   }

   *dmemTotal = memTab[ 0 ];
   *dmemFree  = memTab[ 1 ];
   *dbuffer   = memTab[ 2 ];
   *dcached   = memTab[ 3 ];

   procFile.close();
}

void CMemoryLoad::read()
{
   unsigned long long memTotal  =0;
   unsigned long long memFree   =0;
   unsigned long long memBuffer =0;
   unsigned long long memCached =0;

   ReadFromFile ( &memTotal, &memFree, &memBuffer, &memCached );

   // as described here :
   // http://blog.guillaume.fenollar.fr/2013/11/comprendre-conso-memoire-vive-ram-linux.html

   float MemoryLoad = static_cast<float>(((memTotal-memFree)-memBuffer-memCached) * 100 / float(memTotal));

   m_keyword->set( MemoryLoad );

   std::cout << "Memory Load : " << m_keyword->formatValue() << std::endl;
}
