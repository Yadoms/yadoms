#include "stdafx.h"
#include "IdentityForQualifier.h"


namespace pluginSystem
{

CIdentityForQualifier::CIdentityForQualifier(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   :m_type(pluginInformation->getType()), m_version(pluginInformation->getVersion()), m_releaseType(pluginInformation->getReleaseType())
{
}

CIdentityForQualifier::CIdentityForQualifier(const std::string& type, const std::string& version, shared::versioning::EReleaseType releaseType)
   : m_type(type), m_version(version), m_releaseType(releaseType)
{
}

CIdentityForQualifier::~CIdentityForQualifier()
{
}

const std::string& CIdentityForQualifier::getType() const
{
   return m_type;
}

const std::string& CIdentityForQualifier::getVersion() const
{
   return m_version;
}

shared::versioning::EReleaseType CIdentityForQualifier::getReleaseType() const
{
   return m_releaseType;
}

bool CPluginIdentityCompare::operator() (const CIdentityForQualifier& lhs, const CIdentityForQualifier& rhs) const
{
   if (lhs.getType() != rhs.getType())
      return lhs.getType() < rhs.getType();
   if (lhs.getVersion() != rhs.getVersion())
      return lhs.getVersion() < rhs.getVersion();
   return lhs.getReleaseType() < rhs.getReleaseType();
}

} // namespace pluginSystem
