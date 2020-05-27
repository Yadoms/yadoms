#include "stdafx.h"
#include "Information.h"
#include "tools/SupportedPlatformsChecker.h"
#include "InvalidPluginException.hpp"

namespace pluginSystem
{
   CInformation::CInformation(const boost::filesystem::path& pluginPath)
      : m_path(pluginPath),
        m_isSupportedOnThisPlatform(true),
        m_package(shared::CDataContainer::make())
   {
      try
      {
         const auto packageFile = m_path / "package.json";
         m_package->deserializeFromFile(packageFile.string());
      }
      catch (shared::exception::CException& e)
      {
         throw shared::exception::CInvalidParameter(pluginPath.stem().string() + std::string(" : Error reading package.json : ") + e.what());
      }

      try
      {
         // Get and check data

         m_type = m_package->get<std::string>("type");
         if (m_type.empty())
            throw shared::exception::CInvalidParameter("Error reading package.json : plugin type can not be empty");

         try
         {
            m_version = shared::versioning::CSemVer(m_package->get<std::string>("version"));
         }
         catch (std::exception&)
         {
            throw shared::exception::CInvalidParameter("Error reading package.json : plugin version doesn't match expected SEMVER format (x.x.x)");
         }

         m_author = m_package->get<std::string>("author");
         if (m_author.empty())
            throw shared::exception::CInvalidParameter("Error reading package.json : plugin author can not be empty");

         if (m_package->containsValue("url"))
            m_url = m_package->get<std::string>("url"); // No check on URL
         else
            m_url = std::string();

         if (m_package->containsValue("supportedPlatforms") || m_package->containsChild("supportedPlatforms"))
            m_isSupportedOnThisPlatform = tools::CSupportedPlatformsChecker::isSupported(m_package->get<shared::CDataContainer>("supportedPlatforms"));
         else
            m_isSupportedOnThisPlatform = true;

         if (m_package->containsValue("supportManuallyDeviceCreation"))
            m_supportManuallyCreatedDevice = m_package->get<bool>("supportManuallyDeviceCreation");
         else
            m_supportManuallyCreatedDevice = false;

         if (m_package->containsValue("supportDeviceRemovedNotification"))
            m_supportDeviceRemovedNotification = m_package->get<bool>("supportDeviceRemovedNotification");
         else
            m_supportDeviceRemovedNotification = false;
      }
      catch (shared::exception::CException& e)
      {
         // Set plugin as not supported
         m_isSupportedOnThisPlatform = false;
         throw shared::exception::CInvalidParameter(pluginPath.stem().string() + std::string(" : Error reading package.json : data not found : ") + e.what());
      }

      const auto pluginFolder = m_path.filename().string();
      if (!boost::equal(pluginFolder, m_type))
      {
         // Set plugin as not supported
         m_isSupportedOnThisPlatform = false;
         throw CInvalidPluginException(m_type, (boost::format("The plugin folder '%1%' does not match plugin type '%2%'") % pluginFolder % m_type).str());
      }
   }

   CInformation::~CInformation()
   {
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
      formatedInformations << m_type;
      formatedInformations << " v" << m_version.toString();
      return formatedInformations.str();
   }


   std::string CInformation::toString() const
   {
      // Full informations = identity + author name + url
      std::ostringstream formatedInformations;

      formatedInformations << getIdentity();
      formatedInformations << " by " << m_author;
      formatedInformations << " (" << m_url << ")";

      return formatedInformations.str();
   }

   bool CInformation::isSupportedOnThisPlatform() const
   {
      return m_isSupportedOnThisPlatform;
   }

   bool CInformation::getSupportManuallyCreatedDevice() const
   {
      return m_supportManuallyCreatedDevice;
   }

   bool CInformation::getSupportDeviceRemovedNotification() const
   {
      return m_supportDeviceRemovedNotification;
   }

   boost::shared_ptr<const shared::CDataContainer> CInformation::getPackage() const
   {
      return m_package;
   }

   const boost::filesystem::path& CInformation::getPath() const
   {
      return m_path;
   }
} // namespace pluginSystem
