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
            m_isSupportedOnThisPlatform =
               tools::CSupportedPlatformsChecker::isSupported(m_package->get<shared::CDataContainer>("supportedPlatforms"));
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

         if (m_package->containsChild("configurationSchema"))
            m_configurationSchema = m_package->getChild("configurationSchema");
         else
            m_configurationSchema = shared::CDataContainer::EmptyContainerSharedPtr;

         if (m_package->containsChild("deviceConfiguration.staticConfigurationSchema"))
            m_deviceStaticConfigurationSchema = m_package->getChild("deviceConfiguration.staticConfigurationSchema");
         else
            m_deviceStaticConfigurationSchema = shared::CDataContainer::EmptyContainerSharedPtr;

         if (m_package->containsChild("deviceConfiguration.dynamicConfigurationSchema"))
            m_deviceDynamicConfigurationSchema = m_package->getChild("deviceConfiguration.dynamicConfigurationSchema");
         else
            m_deviceDynamicConfigurationSchema = shared::CDataContainer::EmptyContainerSharedPtr;
      }
      catch (shared::exception::CException& e)
      {
         // Set plugin as not supported
         m_isSupportedOnThisPlatform = false;
         throw shared::exception::CInvalidParameter(
            pluginPath.stem().string() + std::string(" : Error reading package.json : data not found : ") + e.what());
      }

      const auto pluginFolder = m_path.filename().string();
      if (!boost::equal(pluginFolder, m_type))
      {
         // Set plugin as not supported
         m_isSupportedOnThisPlatform = false;
         throw CInvalidPluginException(
            m_type, (boost::format("The plugin folder '%1%' does not match plugin type '%2%'") % pluginFolder % m_type).str());
      }
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
      // Full information = identity + author name + url
      std::ostringstream formatedInformation;

      formatedInformation << getIdentity();
      formatedInformation << " by " << m_author;
      formatedInformation << " (" << m_url << ")";

      return formatedInformation.str();
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

   boost::shared_ptr<const shared::CDataContainer> CInformation::getConfigurationSchema() const
   {
      return m_configurationSchema;
   }

   boost::shared_ptr<const shared::CDataContainer> CInformation::getDeviceStaticConfigurationSchema() const
   {
      return m_deviceStaticConfigurationSchema;
   }

   boost::shared_ptr<const shared::CDataContainer> CInformation::getDeviceDynamicConfigurationSchema() const
   {
      return m_deviceDynamicConfigurationSchema;
   }

   boost::shared_ptr<const shared::CDataContainer> CInformation::getLabels(const std::vector<std::string>& locales) const
   {
      if (locales.empty())
         return shared::CDataContainer::make();

      for (const auto& locale : locales)
      {
         const auto labels = m_labels.find(locale);
         if (labels != m_labels.end())
            return labels->second;

         try
         {
            const auto labelFilePath = getPath() / std::string("locales/" + locale + ".json");
            if (!boost::filesystem::exists(labelFilePath))
               continue;

            m_labels[locale] = shared::CDataContainer::make(labelFilePath);
            return m_labels[locale];
         }
         catch (const std::exception&)
         {
         }
      }

      try
      {
         return shared::CDataContainer::make(getPath() / std::string("locales/en.json"));
      }
      catch (const std::exception&)
      {
         throw std::invalid_argument(
            "Unable to find valid locale file for plugin " + getType() +
            ". Tried requested locales, and 'en' as default");
      }
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
