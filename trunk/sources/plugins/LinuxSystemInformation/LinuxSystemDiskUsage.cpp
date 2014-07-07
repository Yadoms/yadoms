#include "stdafx.h"
#include "LinuxSystemDiskUsage.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/StandardUnits.h>

CLinuxSystemDiskUsage::CLinuxSystemDiskUsage(const std::string & deviceId, const std::string & driveName, const std::string & Keyword)
   :m_deviceId(deviceId), m_driveName(driveName), m_Keyword(Keyword), m_Capacity("DiskUsage")
{
}

CLinuxSystemDiskUsage::~CLinuxSystemDiskUsage()
{}

const std::string& CLinuxSystemDiskUsage::getDeviceId() const
{
   return m_deviceId;
}

const std::string& CLinuxSystemDiskUsage::getCapacity() const
{
   return m_Capacity;
}

const std::string& CLinuxSystemDiskUsage::getKeyword() const
{
   return m_Keyword;
}

const std::string& CLinuxSystemDiskUsage::getDriveName() const
{
   return m_driveName;
}

void CLinuxSystemDiskUsage::declareDevice(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Declare the device
   context->declareDevice(m_deviceId, shared::CStringExtension::EmptyString, shared::CStringExtension::EmptyString);

   // Declare associated keywords (= values managed by this device)
   context->declareCustomKeyword(m_deviceId, getKeyword(), getCapacity(), yApi::kReadOnly , yApi::kDecimal, shared::plugin::yadomsApi::CStandardUnits::Percent);
}

void CLinuxSystemDiskUsage::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");
   context->historizeData(m_deviceId, getKeyword()  , m_diskUsage);
}

std::vector<std::string> CLinuxSystemDiskUsage::ExecuteCommandAndReturn(const std::string &szCommand)
{
   std::vector<std::string> ret;

   FILE *fp;
   char path[1035];

   /* Open the command for reading. */

   fp = popen(szCommand.c_str(), "r");

   if (fp != NULL) 
   {
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

double CLinuxSystemDiskUsage::getValue()
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
             return 100*usedblocks / double(numblock);
         }
      }
   }
}
