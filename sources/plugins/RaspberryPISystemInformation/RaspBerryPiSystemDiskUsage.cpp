#include "stdafx.h"
#include "RaspBerryPiSystemDiskUsage.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

CRaspBerryPiSystemDiskUsage::CRaspBerryPiSystemDiskUsage(const std::string & device, const std::string & driveName, const std::string & keywordName)
   :m_device(device), m_driveName(driveName), m_keyword(keywordName)
{
}

CRaspBerryPiSystemDiskUsage::~CRaspBerryPiSystemDiskUsage()
{}

void CRaspBerryPiSystemDiskUsage::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Declare associated keywords (= values managed by this device)
   context->declareKeyword(m_device, m_keyword);
}

void CRaspBerryPiSystemDiskUsage::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   context->historizeData(m_device, m_keyword);
}

std::vector<std::string> CRaspBerryPiSystemDiskUsage::ExecuteCommandAndReturn(const std::string &szCommand)
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

void CRaspBerryPiSystemDiskUsage::read()
{
   std::vector<std::string> _rlines=ExecuteCommandAndReturn("df");
   std::vector<std::string>::const_iterator iteratorCommandDF;

   for (iteratorCommandDF=_rlines.begin(); iteratorCommandDF!=_rlines.end(); ++iteratorCommandDF)
   {
      char dname[200];
      char suse[30];
      char smountpoint[300];
      long numblock, usedblocks, availblocks;
      int ret=sscanf((*iteratorCommandDF).c_str(), "%s\t%ld\t%ld\t%ld\t%s\t%s\n", dname, &numblock, &usedblocks, &availblocks, suse, smountpoint);
      if (ret==6) // TODO : Comprendre pourquoi 6
      {
         if (strstr(dname,m_driveName.c_str())!=NULL)
         {
            //YADOMS_LOG(debug) << "usedblocks :" << usedblocks;
            //YADOMS_LOG(debug) << "numblock   :" << numblock;
            m_keyword.set (usedblocks/double(numblock)*100);
            //YADOMS_LOG(debug) << "diskUsage  :" << m_keyword.formatValue();
			YADOMS_LOG(debug) << "LinuxSystemDiskUsage plugin :  " << m_driveName << " Disk Usage : " << m_keyword.formatValue();
         }
      }
   }
}
