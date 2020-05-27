#include "stdafx.h"
#include "IdentityForQualifier.h"


namespace pluginSystem
{

CIdentityForQualifier::CIdentityForQualifier(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   :m_type(pluginInformation->getType()), m_version(pluginInformation->getVersion())
{
}

CIdentityForQualifier::CIdentityForQualifier(const std::string& type, const shared::versioning::CSemVer& version)
   : m_type(type), m_version(version)
{
}

CIdentityForQualifier::~CIdentityForQualifier()
{
}

const std::string& CIdentityForQualifier::getType() const
{
   return m_type;
}

const shared::versioning::CSemVer& CIdentityForQualifier::getVersion() const
{
   return m_version;
}


bool CPluginIdentityCompare::operator() (const CIdentityForQualifier& lhs, const CIdentityForQualifier& rhs) const
{
   if (lhs.getType() != rhs.getType())
      return lhs.getType() < rhs.getType();
   return lhs.getVersion() < rhs.getVersion();
}

} // namespace pluginSystem
