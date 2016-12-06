#include "stdafx.h"
#include "RunningInformation.h"
#include <Poco/Environment.h>
#include <shared/currentTime/Provider.h>
#include "Version.h"

CRunningInformation::CRunningInformation(const std::string & path)
   :m_startupDateTime(shared::currentTime::Provider().now()),
   m_softwareVersion(shared::versioning::CVersion(YadomsVersion), YadomsReleaseType), m_executablePath(path),
   m_serverFullyLoaded(false)
{
}

CRunningInformation::~CRunningInformation()
{
}


const boost::posix_time::ptime  & CRunningInformation::getStartupDateTime() const
{
   return m_startupDateTime;
}
   

const shared::versioning::CVersionInformation  & CRunningInformation::getSoftwareVersion() const
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

void CRunningInformation::setServerFullyLoaded()
{
   m_serverFullyLoaded = true;
}

bool CRunningInformation::isServerFullyLoaded() const
{
   return m_serverFullyLoaded;
}