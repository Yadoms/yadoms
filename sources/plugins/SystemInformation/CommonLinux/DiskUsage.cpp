#include "stdafx.h"
#include "DiskUsage.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include "LinuxHelpers.h"
#include "Helpers.h"

CDiskUsage::CDiskUsage(const std::string& keywordName,
                       const std::string& driveName)
   : m_driveName(driveName),
     m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
}

CDiskUsage::~CDiskUsage()
{
}

void CDiskUsage::read()
{
   std::vector<std::string> _rlines=ExecuteCommandAndReturn("df");
   std::vector<std::string>::const_iterator iteratorCommandDF;

   for (iteratorCommandDF = _rlines.begin(); iteratorCommandDF != _rlines.end(); ++iteratorCommandDF)
   {
      if (strstr((*iteratorCommandDF).c_str(), m_driveName.c_str()) != NULL)
      {
         boost::regex reg("([^\\s]+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)");
         boost::smatch match;

         if (boost::regex_search(*iteratorCommandDF, match, reg))
         {
             long long numblock    = boost::lexical_cast<long long>(match[2]);
             long long availblocks = boost::lexical_cast<long long>(match[4]);
			 
             m_keyword->set(valueRoundWithPrecision((numblock - availblocks)/double(numblock)*100,3));
         }
       }
   }
}

