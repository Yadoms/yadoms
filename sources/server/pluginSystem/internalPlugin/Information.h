#pragma once

#include <shared/plugin/information/IInformation.h>

namespace pluginSystem
{
   namespace internalPlugin
   {
      //--------------------------------------------------------------
      /// \brief		Container class for information about internal plugins
      //--------------------------------------------------------------
      class CInformation : public shared::plugin::information::IInformation
      {
      public:
         CInformation();
         virtual ~CInformation();

         // shared::plugin::IInformation implementation
         const std::string& getType() const override;
         const shared::versioning::CSemVer& getVersion() const override;
         const std::string& getAuthor() const override;
         const std::string& getUrl() const override;
         std::string toString() const override;
         std::string getIdentity() const override;
         bool isSupportedOnThisPlatform() const override;
         bool getSupportManuallyCreatedDevice() const override;
         bool getSupportDeviceRemovedNotification() const override;
         shared::CDataContainerSharedPtrConst getPackage() const override;
         const boost::filesystem::path& getPath() const override;
         // [END] shared::plugin::IInformation implementation

      private:
         std::string m_type;
         shared::versioning::CSemVer m_version;
         std::string m_author;
         std::string m_url;
         shared::CDataContainerSharedPtr m_package;
      };
   }
} // namespace pluginSystem::internalPlugin


