#include "stdafx.h"
#include "System.h"
#include "tools/OperatingSystem.h"

CSystem::CSystem()
   :m_startupDateTime(boost::posix_time::second_clock::universal_time()),
   m_softwareVersion(1,0,0,0)
{
}

CSystem::~CSystem()
{
}


const boost::posix_time::ptime  & CSystem::getStartupDateTime() const
{
   return m_startupDateTime;
}
   

const tools::CVersion  & CSystem::getSoftwareVersion() const
{
   return m_softwareVersion;
}   


const std::string & CSystem::getOperatingSystemName() const
{
   return tools::COperatingSystem::getName();
}