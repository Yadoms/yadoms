#include "stdafx.h"
#include "YadomsSystemInternal.h"
#include "tools/OperatingSystem.h"

CYadomsSystemInternal::CYadomsSystemInternal()
   :m_startupDateTime(boost::posix_time::second_clock::universal_time()),
   m_softwareVersion(1,0,0,0)
{
}

CYadomsSystemInternal::~CYadomsSystemInternal()
{
}


const boost::posix_time::ptime  & CYadomsSystemInternal::getStartupDateTime() const
{
   return m_startupDateTime;
}
   

const tools::CVersion  & CYadomsSystemInternal::getSoftwareVersion() const
{
   return m_softwareVersion;
}   


const std::string & CYadomsSystemInternal::getOperatingSystemName() const
{
   return tools::COperatingSystem::getName();
}