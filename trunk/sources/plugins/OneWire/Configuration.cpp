#include "stdafx.h"
#include "Configuration.h"
#include <shared/Log.h>

CConfiguration::CConfiguration()
   :m_owfsMountPoint("/mnt/1wire")
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
      m_owfsMountPoint = boost::filesystem::path(data.get<std::string>("OwfsMountPoint"));
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