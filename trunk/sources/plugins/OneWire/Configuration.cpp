#include "stdafx.h"
#include "Configuration.h"
#include <shared/Log.h>

CConfiguration::CConfiguration()
   :m_isOwfsMode(false), m_isKernelMode(false)
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
      if (data.exists("linuxMode"))
      {
         m_isOwfsMode = data.get<bool>("linuxMode.content.owfs.radio");
         if (m_isOwfsMode)
            m_owfsMountPoint = boost::filesystem::path(data.get<std::string>("linuxMode.content.owfs.content.mountPoint"));

         m_isKernelMode = data.get<bool>("linuxMode.content.kernel.radio");
         if (m_isKernelMode)
            m_kernelMountPoint = boost::filesystem::path(data.get<std::string>("linuxMode.content.kernel.content.mountPoint"));
      }
   }
   catch (const shared::exception::CException& e)
	{
		YADOMS_LOG(error) << "OneWire configuration could not be loaded, " << e.what();
	}
}

bool CConfiguration::isOwfsMode() const
{
   return m_isOwfsMode;
}

bool CConfiguration::isKernelMode() const;
{
   return m_isKernelMode;
}

const boost::filesystem::path& CConfiguration::getOwfsMountPoint() const
{
   return m_owfsMountPoint;
}

const boost::filesystem::path& CConfiguration::getKernelMountPoint() const
{
   return m_kernelMountPoint;
}
