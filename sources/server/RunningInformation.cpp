#include "stdafx.h"
#include "RunningInformation.h"
#include <Poco/Environment.h>
#include <shared/currentTime/Provider.h>

#include <utility>

CRunningInformation::CRunningInformation(std::string path,
                                         const shared::versioning::CSemVer& yadomsVersion)
   : m_startupDateTime(shared::currentTime::Provider().now()),
   m_softwareVersion(yadomsVersion),
   m_executablePath(std::move(path)),
   m_serverFullyLoaded(false)
{
}


const boost::posix_time::ptime& CRunningInformation::getStartupDateTime() const
{
   return m_startupDateTime;
}


const shared::versioning::CVersionInformation& CRunningInformation::getSoftwareVersion() const
{
   return m_softwareVersion;
}


const std::string& CRunningInformation::getOperatingSystemName() const
{
   static const auto OsName = Poco::Environment::osDisplayName() + " " + Poco::Environment::osVersion();
   return OsName;
}

const std::string& CRunningInformation::getExecutablePath() const
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
