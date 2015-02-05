#include "stdafx.h"
#include "IdentityForQualifier.h"


namespace pluginSystem
{

CIdentityForQualifier::CIdentityForQualifier(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   :m_name(pluginInformation->getName()), m_version(pluginInformation->getVersion()), m_releaseType(pluginInformation->getReleaseType())
{
}

CIdentityForQualifier::CIdentityForQualifier(const std::string& name, const std::string& version, shared::plugin::information::EReleaseType releaseType)
   :m_name(name), m_version(version), m_releaseType(releaseType)
{
}

CIdentityForQualifier::~CIdentityForQualifier()
{
}

const std::string& CIdentityForQualifier::getName() const
{
   return m_name;
}

const std::string& CIdentityForQualifier::getVersion() const
{
   return m_version;
}

const shared::plugin::information::EReleaseType CIdentityForQualifier::getReleaseType() const
{
   return m_releaseType;
}

bool CPluginIdentityCompare::operator() (const CIdentityForQualifier& lhs, const CIdentityForQualifier& rhs) const
{
   if (lhs.getName() != rhs.getName())
      return lhs.getName() < rhs.getName();
   if (lhs.getVersion() != rhs.getVersion())
      return lhs.getVersion() < rhs.getVersion();
   return lhs.getReleaseType() < rhs.getReleaseType();
}

} // namespace pluginSystem
