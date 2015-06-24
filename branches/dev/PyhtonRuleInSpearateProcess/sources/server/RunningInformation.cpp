#include "stdafx.h"
#include "RunningInformation.h"
#include "tools/OperatingSystem.h"
#include <Poco/Environment.h>
#include <shared/Log.h>

CRunningInformation::CRunningInformation(const std::string & path)
   :m_startupDateTime(boost::posix_time::second_clock::universal_time()),
   m_softwareVersion(1, 0, 0, 0), m_executablePath(path)
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

const std::string & CRunningInformation::getExecutablePath() const
{
   return m_executablePath;
}