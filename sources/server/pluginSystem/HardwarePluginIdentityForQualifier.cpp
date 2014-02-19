#include "stdafx.h"
#include "HardwarePluginIdentityForQualifier.h"


CPluginIdentity::CPluginIdentity(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation)
   :m_name(pluginInformation->getName()), m_version(pluginInformation->getVersion()), m_releaseType(pluginInformation->getReleaseType())
{
}

CPluginIdentity::CPluginIdentity(const std::string& name, const std::string& version, shared::plugin::IInformation::EReleaseType releaseType)
   :m_name(name), m_version(version), m_releaseType(releaseType)
{
}

CPluginIdentity::~CPluginIdentity()
{
}

const std::string& CPluginIdentity::getName() const
{
   return m_name;
}

const std::string& CPluginIdentity::getVersion() const
{
   return m_version;
}

const shared::plugin::IInformation::EReleaseType CPluginIdentity::getReleaseType() const
{
   return m_releaseType;
}

bool CPluginIdentityCompare::operator() (const CPluginIdentity& lhs, const CPluginIdentity& rhs) const
{
   if (lhs.getName() != rhs.getName())
      return lhs.getName() < rhs.getName();
   if (lhs.getVersion() != rhs.getVersion())
      return lhs.getVersion() < rhs.getVersion();
   return lhs.getReleaseType() < rhs.getReleaseType();
}
