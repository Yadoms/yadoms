#include "stdafx.h"
#include "RunningInformation.h"
#include "tools/OperatingSystem.h"
#include <Poco/Environment.h>

CRunningInformation::CRunningInformation()
   :m_startupDateTime(boost::posix_time::second_clock::universal_time()),
   m_softwareVersion(1,0,0,0)
{
}

CRunningInformation::~CRunningInformation()
{
}


const boost::posix_time::ptime  & CRunningInformation::getStartupDateTime() const
{
   return m_startupDateTime;
}
   

const tools::CVersion  & CRunningInformation::getSoftwareVersion() const
{
   return m_softwareVersion;
}   


const std::string CRunningInformation::getOperatingSystemName() const
{
   //do not return references
   return (boost::format("%1% %2%") % Poco::Environment::osDisplayName() % Poco::Environment::osVersion()).str();
}