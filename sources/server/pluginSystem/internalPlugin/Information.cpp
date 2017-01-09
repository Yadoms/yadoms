#include "stdafx.h"
#include "Information.h"

namespace pluginSystem
{
   namespace internalPlugin
   {
      CInformation::CInformation()
         : m_type("system"),
           m_version("1.0.0"),
           m_author("Yadoms team"),
           m_url("http://www.yadoms.com"),
           m_package(boost::make_shared<shared::CDataContainer>())
      {
         m_package->set("type", m_type);
         m_package->set("version", m_version.toString());
         m_package->set("author", m_author);
         m_package->set("url", m_url);
      }

      CInformation::~CInformation()
      {
      }

      const std::string& CInformation::getType() const
      {
         return m_type;
      }

      const shared::versioning::CVersion& CInformation::getVersion() const
      {
         return m_version;
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
         formatedInformations << " v" << getVersion().toString();
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

      bool CInformation::getSupportDeviceRemovedNotification() const
      {
         return false;
      }

      boost::shared_ptr<const shared::CDataContainer> CInformation::getPackage() const
      {
         return m_package;
      }

      const boost::filesystem::path& CInformation::getPath() const
      {
         static const boost::filesystem::path dummy;
         return dummy;
      }
   }
} // namespace pluginSystem::internalPlugin
