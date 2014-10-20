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
      virtual const std::string& getName() const;
      virtual const std::string& getDescription() const;
      virtual const std::string& getVersion() const;
      virtual const shared::plugin::information::EReleaseType getReleaseType() const;
      virtual const std::string& getAuthor() const;
      virtual const std::string& getUrl() const;
      virtual std::string toString() const;
      virtual std::string getIdentity() const;
      // [END] shared::plugin::IInformation implementation

   private:
      std::string m_name;
      std::string m_description;
      std::string m_version;
      shared::plugin::information::EReleaseType m_releaseType;
      std::string m_author;
      std::string m_url;
   };

}} // namespace pluginSystem::internalPlugin
