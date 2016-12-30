#pragma once
#include <shared/script/yInterpreterApi/IInformation.h>
#include <shared/DataContainer.h>

namespace automation
{
   namespace interpreter
   {
      //--------------------------------------------------------------
      /// \brief		Container class for interpreter information
      //--------------------------------------------------------------
      class CInformation : public shared::script::yInterpreterApi::IInformation
      {
      public:
         explicit CInformation(const boost::filesystem::path& interpreterPath);
         virtual ~CInformation();

         // shared::script::yInterpreterApi::IInformation implementation
         const std::string& getType() const override;
         const std::string& getName() const override;
         const std::string& getDescription() const override;
         const std::string& getVersion() const override;
         shared::versioning::EReleaseType getReleaseType() const override;
         const std::string& getAuthor() const override;
         const std::string& getUrl() const override;
         const std::string& getCredits() const override;
         bool isSupportedOnThisPlatform() const override;
         boost::shared_ptr<const shared::CDataContainer> getPackage() const override;
         const boost::filesystem::path& getPath() const override;
         // [END] shared::script::yInterpreterApi::IInformation implementation

      private:
         std::string m_type;
         std::string m_name;
         std::string m_description;
         std::string m_version;
         shared::versioning::EReleaseType m_releaseType;
         std::string m_author;
         std::string m_url;
         std::string m_credits;
         bool m_isSupportedOnThisPlatform;
         boost::shared_ptr<shared::CDataContainer> m_package;
         const boost::filesystem::path m_path;
      };
   }
} // namespace automation::interpreter


