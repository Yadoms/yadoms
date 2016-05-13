#include "stdafx.h"
#include "DiskUsage.h"
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include "LinuxHelpers.h"

CDiskUsage::CDiskUsage(const std::string & device, const std::string & driveName, const std::string & keywordName)
   :m_device(device), 
    m_driveName(driveName), 
    m_keyword( new yApi::historization::CLoad(keywordName) )
{}

CDiskUsage::~CDiskUsage()
{}

void CDiskUsage::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
   if (!api->keywordExists( m_device, m_keyword->getKeyword()))
      api->declareKeyword(m_device, *m_keyword, details);
}

void CDiskUsage::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   BOOST_ASSERT_MSG(api, "api must be defined");

   api->historizeData(m_device, *m_keyword);
}

void CDiskUsage::read()
{
   std::vector<std::string> _rlines=ExecuteCommandAndReturn("df");
   std::vector<std::string>::const_iterator iteratorCommandDF;

   for (iteratorCommandDF=_rlines.begin(); iteratorCommandDF!=_rlines.end(); ++iteratorCommandDF)
   {
      if (strstr((*iteratorCommandDF).c_str(),m_driveName.c_str())!=NULL)
      {
         boost::regex reg("([^\\s]+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)");
         boost::smatch match;

         if ( boost::regex_search( *iteratorCommandDF, match, reg ) )
         {
             long long numblock    = boost::lexical_cast<long long>(match[2]);
             long long availblocks = boost::lexical_cast<long long>(match[4]);

                std::cout << "numblock    :  " << numblock << std::endl;
                std::cout << "availblocks :  " << availblocks << std::endl;
			 
             m_keyword->set ((numblock - availblocks)/double(numblock)*100);
             std::cout << "Disk Name :  " << m_driveName << " Disk Usage : " << m_keyword->formatValue() << std::endl;
         }
       }
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CDiskUsage::GetHistorizable() const
{
	return m_keyword;
}
