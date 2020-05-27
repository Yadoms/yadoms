#include "stdafx.h"
#include "Information.h"
#include "tools/SupportedPlatformsChecker.h"

namespace automation
{
   namespace interpreter
   {
      CInformation::CInformation(const boost::filesystem::path& interpreterPath)
         : m_isSupportedOnThisPlatform(true),
           m_package(shared::CDataContainer::make()),
           m_path(interpreterPath)
      {
         try
         {
            boost::filesystem::path packageFile;
            packageFile = m_path / "package.json";
            m_package->deserializeFromFile(packageFile.string());
         }
         catch (shared::exception::CException& e)
         {
            throw std::runtime_error(m_path.stem().string() + std::string(" : Error reading package.json : ") + e.what());
         }

         try
         {
            // Get and check data

            m_type = m_path.stem().string();

            m_name = m_package->get<std::string>("name");
            if (m_name.empty())
               throw std::invalid_argument("Error reading package.json : interpreter name can not be empty");

            if (m_package->containsValue("description"))
               m_description = m_package->get<std::string>("description");

            m_version = shared::versioning::CSemVer(m_package->get<std::string>("version"));

            m_author = m_package->get<std::string>("author");
            if (m_author.empty())
               throw std::invalid_argument("Error reading package.json : interpreter author can not be empty");

            if (m_package->containsValue("url"))
               m_url = m_package->get<std::string>("url"); // No check on URL

            if (m_package->containsValue("credits"))
               m_credits = m_package->get<std::string>("credits");

            if (m_package->containsValue("supportedPlatforms") || m_package->containsChild("supportedPlatforms"))
               m_isSupportedOnThisPlatform = tools::CSupportedPlatformsChecker::isSupported(m_package->get<shared::CDataContainer>("supportedPlatforms"));
            else
               m_isSupportedOnThisPlatform = true;
         }
         catch (shared::exception::CException& e)
         {
            // Set interpreter as not supported
            m_isSupportedOnThisPlatform = false;
            throw std::invalid_argument(m_path.stem().string() + std::string(" : Error reading package.json : data not found : ") + e.what());
         }

         auto interpreterFolder = m_path.filename().string();
         if (!boost::equal(interpreterFolder, m_type))
         {
            // Set interpreter as not supported
            m_isSupportedOnThisPlatform = false;
            throw std::runtime_error((boost::format("The interpreter folder '%1%' does not match interpreter type '%2%'") % interpreterFolder % m_type).str());
         }
      }

      CInformation::~CInformation()
      {
      }

      const std::string& CInformation::getType() const
      {
         return m_type;
      }

      const std::string& CInformation::getName() const
      {
         return m_name;
      }

      const std::string& CInformation::getDescription() const
      {
         return m_description;
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

      const std::string& CInformation::getCredits() const
      {
         return m_credits;
      }

      bool CInformation::isSupportedOnThisPlatform() const
      {
         return m_isSupportedOnThisPlatform;
      }

      boost::shared_ptr<const shared::CDataContainer> CInformation::getPackage() const
      {
         return m_package;
      }

      const boost::filesystem::path& CInformation::getPath() const
      {
         return m_path;
      }
   }
} // namespace automation::interpreter


