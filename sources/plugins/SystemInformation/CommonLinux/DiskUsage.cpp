#include "stdafx.h"
#include "DiskUsage.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>

CDiskUsage::CDiskUsage(const std::string & device, const std::string & driveName, const std::string & keywordName)
   :m_device(device), m_driveName(driveName), m_keyword(keywordName)
{
}

CDiskUsage::~CDiskUsage()
{}

void CDiskUsage::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_keyword);
}

void CDiskUsage::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_device, m_keyword);
}

std::vector<std::string> CDiskUsage::ExecuteCommandAndReturn(const std::string &szCommand)
{
   std::vector<std::string> ret;

   FILE *fp;

   /* Open the command for reading. */

   fp = popen(szCommand.c_str(), "r");

   if (fp != NULL) 
   {
      char path[1035];
      /* Read the output a line at a time - output it. */
      while (fgets(path, sizeof(path)-1, fp) != NULL)
      {
         ret.push_back(path);
      }
      /* close */
      pclose(fp);
   }
   return ret;
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
			 
             m_keyword.set ((numblock - availblocks)/double(numblock)*100);
             YADOMS_LOG(debug) << "Disk Name :  " << m_driveName << " Disk Usage : " << m_keyword.formatValue();			 
         }
       }
   }
}
