#include "stdafx.h"
#include "Information.h"

namespace pluginSystem {   namespace internalPlugin {

   CInformation::CInformation()
      :m_name("system"), m_description("system"), m_version("1.0"), m_releaseType(shared::plugin::information::EReleaseType::kStable), 
      m_author("Yadoms team"), m_url("http://www.yadoms.com")
   {
   }

   CInformation::~CInformation()
   {
   }


   const std::string& CInformation::getName() const
   {
      return m_name;
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

      formatedInformations << getName();
      formatedInformations << " v" << getVersion();
      formatedInformations << "[stable]";

      return formatedInformations.str();
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

}} // namespace pluginSystem::internalPlugin