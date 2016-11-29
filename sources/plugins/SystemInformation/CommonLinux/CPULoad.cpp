#include "stdafx.h"
#include "CPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <fstream>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>

CCPULoad::CCPULoad(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
   ReadFromFile(m_lastTotalUser,
                m_lastTotalUserLow,
                m_lastTotalSys,
                m_lastTotalIdle,
                m_lastTotalIowait,
                m_lastTotalIrq,
                m_lastTotalSoftIrq);
}

CCPULoad::~CCPULoad()
{
}

void CCPULoad::ReadFromFile(unsigned long long& dtotalUser,
                            unsigned long long& dtotalUserLow,
                            unsigned long long& dtotalSys,
                            unsigned long long& dtotalIdle,
                            unsigned long long& dtotalIowait,
                            unsigned long long& dtotalIrq,
                            unsigned long long& dtotalSoftIrq)
{
   std::ifstream procFile("/proc/stat");

   // This might broke if there are not 7 columns minimum in /proc/stat
   
   // 1 : user.
   // 2 : nice.
   // 3 : system.
   // 4 : idle.
   // 5 : iowait.
   // 6 : irq.
   // 7 : softIrq.
   
   // Only the first CPU is read. All cores are resumed in the cpu line. For each core you have a specific line cpu0, cpu1 ...
   
   boost::regex reg("[cpu ] (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+) (\\d+)");

   //each counters (64 bits) is incremented each time of the number of ticks corresponding. counters should be equal or greater at each reads. if not, it's the overflow of the counter.

   std::string line;
   while ( std::getline(procFile, line) ) 
   {
      boost::smatch match;
      if ( boost::regex_search( line, match, reg ) ) 
      {
          dtotalUser    = boost::lexical_cast<long long>(match[1]);
          dtotalUserLow = boost::lexical_cast<long long>(match[2]);
          dtotalSys     = boost::lexical_cast<long long>(match[3]);
          dtotalIdle    = boost::lexical_cast<long long>(match[4]);
          dtotalIowait  = boost::lexical_cast<long long>(match[5]);
          dtotalIrq     = boost::lexical_cast<long long>(match[6]);
          dtotalSoftIrq = boost::lexical_cast<long long>(match[7]);
      }
   }

   procFile.close();
}

void CCPULoad::read()
{
   unsigned long long totalUser,
                      totalUserLow,
                      totalSys,
                      totalIdle,
                      totalIowait,
                      totalIrq,
                      totalSoftIrq;

   ReadFromFile(totalUser,
                totalUserLow,
                totalSys,
                totalIdle,
                totalIowait,
                totalIrq,
                totalSoftIrq);

   if (totalUser    < m_lastTotalUser || 
       totalUserLow < m_lastTotalUserLow || 
       totalSys     < m_lastTotalSys || 
       totalIdle    < m_lastTotalIdle ||
       totalIowait  < m_lastTotalIowait ||
       totalIrq     < m_lastTotalIrq ||
       totalSoftIrq < m_lastTotalSoftIrq
	   )
   {
     //Overflow detection. Just skip this value.
   }
   else
   {
      double total = (totalUser - m_lastTotalUser) + 
	             (totalUserLow - m_lastTotalUserLow) + 
                     (totalSys - m_lastTotalSys) +
                     (totalIrq - m_lastTotalIrq) +
                     (totalSoftIrq - m_lastTotalSoftIrq);
      double percent = total;
      total = total + (totalIdle - m_lastTotalIdle) +
	              (totalIowait - m_lastTotalIowait);
                 
      if ((total) >0)
      {
         percent /= total;
         percent *= 100;
         m_keyword->set (percent);
         std::cout << "CPU Load : " << m_keyword->get() << std::endl;
      }
      else 
      {
         std::cout << "CPU Load : time too short between execution" << std::endl;
      }
    }
    m_lastTotalUser    = totalUser;
    m_lastTotalUserLow = totalUserLow;
    m_lastTotalSys     = totalSys;
    m_lastTotalIdle    = totalIdle;
    m_lastTotalIowait  = totalIowait;
    m_lastTotalIrq     = totalIrq;
    m_lastTotalSoftIrq = totalSoftIrq;
}
