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
           m_package(createPackage())
      {
         m_package->set("type", m_type);
         m_package->set("version", m_version.toString());
         m_package->set("author", m_author);
         m_package->set("url", m_url);
      }

      const std::string& CInformation::getType() const
      {
         return m_type;
      }

      const shared::versioning::CSemVer& CInformation::getVersion() const
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

      boost::shared_ptr<const shared::CDataContainer> CInformation::getConfigurationSchema() const
      {
         return shared::CDataContainer::EmptyContainerSharedPtr;
      }

      boost::shared_ptr<const shared::CDataContainer> CInformation::getLabels(const std::string& locale) const
      {
         if (locale.empty())
            return shared::CDataContainer::make();

         if (locale == "fr")
         {
            return shared::CDataContainer::make(std::string(
               R"({
                 "recipientFields": {
                   "email": {
                     "name": "Email",
                     "description": "L'adresse email du destinataire",
                     "regexErrorMessage": "Doit correspondre à une adresse email valide"
                   },
                   "mobile" : {
                     "name": "Mobile",
                     "description": "Le numéro de téléphone mobile du destinataire : (format international : +33601020304) ",
                     "regexErrorMessage": "Doit correspondre à un numéro de téléphone valide : commence par +, pas d'espace, de tiret, de () "
                   }
                 }
               })"));
         }

         if (locale == "en")
         {
            return shared::CDataContainer::make(std::string(
               R"({
                 "recipientFields": {
                   "email": {
               "name": "Email (system) ",
               "description": "Recipient email (system) ",
               "regexErrorMessage": "It must match a valid email address"
                   },
                   "mobile" : {
               "name": "Mobile phone (system) ",
               "description": "The mobile phone number of the recipient (system) : (international format : +33601020304) ",
               "regexErrorMessage": "It must match a valid phone number (beginning with +, no space, no -) "
                   }
                 }
               })"));
         }

         return shared::CDataContainer::make();
      }

      boost::shared_ptr<const shared::CDataContainer> CInformation::getPackage() const
      {
         return m_package;
      }

      const boost::filesystem::path& CInformation::getPath() const
      {
         static const boost::filesystem::path Dummy;
         return Dummy;
      }

      boost::shared_ptr<shared::CDataContainer> CInformation::createPackage() const
      {
         auto package = shared::CDataContainer::make(std::string(
            R"({
                 "recipientFields": {
                   "email": {
                     "type": "string",
                     "regex" : "^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+.[a-zA-Z0-9-.]+$"
                   },
                   "mobile" : {
                     "type": "string",
                     "regex" : "^\\+(?:[0-9]?){6,14}[0-9]$"
                   }
                 }
               })"));

         return package;
      }
   }
} // namespace pluginSystem::internalPlugin
