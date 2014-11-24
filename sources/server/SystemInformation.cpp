#include "stdafx.h"
#include "SystemInformation.h"
#include "tools/OperatingSystem.h"

CSystemInformation::CSystemInformation()
   :m_startupDateTime(boost::posix_time::second_clock::universal_time()),
   m_softwareVersion(1,0,0,0)
{
}

CSystemInformation::~CSystemInformation()
{
}


const boost::posix_time::ptime  & CSystemInformation::getStartupDateTime() const
{
   return m_startupDateTime;
}
   

const tools::CVersion  & CSystemInformation::getSoftwareVersion() const
{
   return m_softwareVersion;
}   


const std::string & CSystemInformation::getOperatingSystemName() const
{
   return tools::COperatingSystem::getName();
}