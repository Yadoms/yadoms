#include "stdafx.h"
#include <shared/Log.h>
#include "CPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <fstream>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>

CCPULoad::CCPULoad(const std::string & device)
   :m_device(device), 
    m_keyword( new yApi::historization::CLoad("CPULoad") )
{
   ReadFromFile ( &m_lastTotalUser, &m_lastTotalUserLow, &m_lastTotalSys, &m_lastTotalIdle);
}

CCPULoad::~CCPULoad()
{
}

void CCPULoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Declare associated keywords (= values managed by this device)
   if (!context->keywordExists( m_device, m_keyword->getKeyword()))
   {
      context->declareKeyword(m_device, *m_keyword);
   }
}

void CCPULoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(!!context, "context must be defined");

   context->historizeData(m_device, *m_keyword);
}

void CCPULoad::ReadFromFile(unsigned long long *dtotalUser,
                            unsigned long long *dtotalUserLow,
                            unsigned long long *dtotalSys,
                            unsigned long long *dtotalIdle)
{
   std::ifstream procFile("/proc/stat");

   //This might broke if there are not 4 columns minimum in /proc/stat
   boost::regex reg("cpu  (\\d+) (\\d+) (\\d+) (\\d+)");

   //each counters (64 bits) is incremented each time of the number of ticks corresponding. counters should be equal or greater at each reads. if not, it's the overflow of the counter.

   std::string line;
   while ( std::getline(procFile, line) ) 
   {
      boost::smatch match;
      if ( boost::regex_search( line, match, reg ) ) 
      {
          *dtotalUser    = boost::lexical_cast<long long>(match[1]);
          *dtotalUserLow = boost::lexical_cast<long long>(match[2]);
          *dtotalSys     = boost::lexical_cast<long long>(match[3]);
          *dtotalIdle    = boost::lexical_cast<long long>(match[4]);
      }
   }

   procFile.close();
}

void CCPULoad::read()
{
   unsigned long long totalUser, totalUserLow, totalSys, totalIdle;

   ReadFromFile ( &totalUser, &totalUserLow, &totalSys, &totalIdle );

   if (totalUser < m_lastTotalUser || totalUserLow < m_lastTotalUserLow || totalSys < m_lastTotalSys || totalIdle < m_lastTotalIdle)
   {
     //Overflow detection. Just skip this value.
   }
   else
   {
      double percent; 
      unsigned long long total = (totalUser - m_lastTotalUser) + (totalUserLow - m_lastTotalUserLow) + (totalSys - m_lastTotalSys);
      percent = total;
      total += (totalIdle - m_lastTotalIdle);
      percent /= total;
      percent *= 100;

      m_keyword->set (percent);
      YADOMS_LOG(debug) << "CPU Load : " << m_keyword->formatValue();
    }
    m_lastTotalUser = totalUser;
    m_lastTotalUserLow = totalUserLow;
    m_lastTotalSys = totalSys;
    m_lastTotalIdle = totalIdle;
}

boost::shared_ptr<yApi::historization::IHistorizable> CCPULoad::GetHistorizable() const
{
	return m_keyword;
}
