#pragma once

#include <shared/plugin/information/IInformation.h>

namespace pluginSystem {   namespace internalPlugin {

   //--------------------------------------------------------------
   /// \brief		Container class for information about internal plugins
   //--------------------------------------------------------------
   class CInformation : public shared::plugin::information::IInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    pluginPath : plugin executing path
      /// \throw        exception::CInvalidParameter if file is not valid or complete
      //--------------------------------------------------------------
      CInformation();

      // shared::plugin::IInformation implementation
      virtual ~CInformation();
      virtual const std::string& getType() const;
      virtual const std::string& getVersion() const;
      virtual const shared::versioning::EReleaseType getReleaseType() const;
      virtual const std::string& getAuthor() const;
      virtual const std::string& getUrl() const;
      virtual std::string toString() const;
      virtual std::string getIdentity() const;
      virtual bool isSupportedOnThisPlatform() const;
      virtual bool getSupportManuallyCreatedDevice() const;
      virtual shared::CDataContainer getPackage() const;
      // [END] shared::plugin::IInformation implementation

   private:
      std::string m_type;
      std::string m_version;
      shared::versioning::EReleaseType m_releaseType;
      std::string m_author;
      std::string m_url;
      shared::CDataContainer m_package;
   };

}} // namespace pluginSystem::internalPlugin
