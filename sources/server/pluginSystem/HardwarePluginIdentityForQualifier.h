//
// CHardwarePluginIdentityForQualifier.h
//
// Yadoms-plugin identity for qualifier internal use
//
#pragma once

#include "IHardwarePluginQualifier.h"

//--------------------------------------------------------------
/// \brief	Internal plugin identity
//--------------------------------------------------------------
class CPluginIdentity
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPluginIdentity(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation);

   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CPluginIdentity(const std::string& name, const std::string& version, shared::plugin::IInformation::EReleaseType releaseType);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CPluginIdentity();

   //--------------------------------------------------------------
   /// \brief	   Name getter
   /// \return    Plugin name
   //--------------------------------------------------------------
   const std::string& getName() const;

   //--------------------------------------------------------------
   /// \brief	   Version getter
   /// \return    Plugin version
   //--------------------------------------------------------------
   const std::string& getVersion() const;

   //--------------------------------------------------------------
   /// \brief	   Release type getter
   /// \return    Plugin release type
   //--------------------------------------------------------------
   const shared::plugin::IInformation::EReleaseType getReleaseType() const;

private:
   const std::string m_name;
   const std::string m_version;
   const shared::plugin::IInformation::EReleaseType m_releaseType;
};

//--------------------------------------------------------------
/// \brief	Compare operator (used in map)
//--------------------------------------------------------------
class CPluginIdentityCompare
{
public:
   bool operator() (const CPluginIdentity& lhs, const CPluginIdentity& rhs) const;
};
