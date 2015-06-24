#include "stdafx.h"
#include "Information.h"

namespace pluginSystem {   namespace internalPlugin {

   CInformation::CInformation()
      :m_type("system"), m_description("system"), m_version("1.0"), m_releaseType(shared::plugin::information::EReleaseType::kStable),
      m_author("Yadoms team"), m_url("http://www.yadoms.com")
   {
      m_package.set("type", m_type);
      m_package.set("description", m_description);
      m_package.set("version", m_version);
      m_package.set("releaseType", m_releaseType);
      m_package.set("author", m_author);
      m_package.set("url", m_url);
   }

   CInformation::~CInformation()
   {
   }


   const std::string& CInformation::getType() const
   {
      return m_type;
   }

   const std::string& CInformation::getDescription() const
   {
      return m_description;
   }
   const std::string& CInformation::getVersion() const
   {
      return m_version;
   }
   const shared::plugin::information::EReleaseType CInformation::getReleaseType() const
   {
      return m_releaseType;
   }

   const std::string& CInformation::getAuthor() const
   {
      return m_author;
   }

   const std::string& CInformation::getUrl() const
   {
      return m_url;
   }

   std::string CInformation::getIdentity() const
   {
      std::ostringstream formatedInformations;

      formatedInformations << getType();
      formatedInformations << " v" << getVersion();
      formatedInformations << "[stable]";

      return formatedInformations.str();
   }

   bool CInformation::isSupportedOnThisPlatform() const
   {
      return true;
   }

   std::string CInformation::toString() const
   {
      // Full informations = identity + author name + url
      std::ostringstream formatedInformations;

      formatedInformations << getIdentity();
      formatedInformations << " by " << getAuthor();
      formatedInformations << " (" << getUrl() << ")";

      return formatedInformations.str();
   }

   bool CInformation::getSupportManuallyCreatedDevice() const
   {
      return false;
   }

   shared::CDataContainer CInformation::getPackageJson() const
   {
      return m_package;
   }

}} // namespace pluginSystem::internalPlugin