//
// IdentityForQualifier.h
//
// Yadoms-plugin identity for qualifier internal use
//
#pragma once

#include "IQualifier.h"
#include <shared/versioning/SemVer.h>

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
      explicit CIdentityForQualifier(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);

      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CIdentityForQualifier(const std::string& name, const shared::versioning::CSemVer& version);

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
      const shared::versioning::CSemVer& getVersion() const;

   private:
      const std::string m_type;
      const shared::versioning::CSemVer m_version;
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
