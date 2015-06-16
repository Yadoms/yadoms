#include "stdafx.h"
#include "Configuration.h"
#include <shared/Log.h>

CConfiguration::CConfiguration()
   :m_owfsMountPoint("/mnt/1wire"), m_kernelMountPoint("/sys/bus/w1/devices")
{
}

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);

   try
   {
      m_owfsMountPoint = boost::filesystem::path(data.get<std::string>("linuxMode.owfs.mountPoint"));
      m_kernelMountPoint = boost::filesystem::path(data.get<std::string>("linuxMode.kernel.mountPoint"));
   }
   catch (const shared::exception::CException& e)
	{
		YADOMS_LOG(error) << "OneWire configuration could not be loaded, " << e.what();
	}
}

const boost::filesystem::path& CConfiguration::getOwfsMountPoint() const
{
   return m_owfsMountPoint;
}

const boost::filesystem::path& CConfiguration::getKernelMountPoint() const
{
   return m_kernelMountPoint;
}