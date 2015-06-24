//
// IdentityForQualifier.h
//
// Yadoms-plugin identity for qualifier internal use
//
#pragma once

#include "IQualifier.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	Internal plugin identity
   //--------------------------------------------------------------
   class CIdentityForQualifier
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CIdentityForQualifier(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);

      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CIdentityForQualifier(const std::string& name, const std::string& version, shared::plugin::information::EReleaseType releaseType);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CIdentityForQualifier();

      //--------------------------------------------------------------
      /// \brief	   Type getter
      /// \return    Plugin type
      //--------------------------------------------------------------
      const std::string& getType() const;

      //--------------------------------------------------------------
      /// \brief	   Version getter
      /// \return    Plugin version
      //--------------------------------------------------------------
      const std::string& getVersion() const;

      //--------------------------------------------------------------
      /// \brief	   Release type getter
      /// \return    Plugin release type
      //--------------------------------------------------------------
      const shared::plugin::information::EReleaseType getReleaseType() const;

   private:
      const std::string m_type;
      const std::string m_version;
      const shared::plugin::information::EReleaseType m_releaseType;
   };

   //--------------------------------------------------------------
   /// \brief	Compare operator (used in map)
   //--------------------------------------------------------------
   class CPluginIdentityCompare
   {
   public:
      bool operator() (const CIdentityForQualifier& lhs, const CIdentityForQualifier& rhs) const;
   };

} // namespace pluginSystem
