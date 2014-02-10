#pragma once

#include <shared/Export.h>
#include <string>

//--------------------------------------------------------------
/// \brief		Interface for information about Hardware plugins
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT IHardwarePluginInformation 
{
public:
   //--------------------------------------------------------------
   /// \brief		Enum for plugin state
   //--------------------------------------------------------------
   enum EReleaseType
   {
      kStable = 0,   /// \brief		plugin is stable
      kTesting,      /// \brief		plugin is in testing phase
      kBeta          /// \brief		plugin is in beta state
   };

public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IHardwarePluginInformation () { }

   //--------------------------------------------------------------
   /// \brief	    get the plugin Name
   /// \return     the plugin name
   //--------------------------------------------------------------
   virtual const std::string& getName() const = 0;

   //--------------------------------------------------------------
   /// \brief	    get the plugin version
   /// \return     the plugin version
   //--------------------------------------------------------------
   virtual const std::string& getVersion() const = 0;

   //--------------------------------------------------------------
   /// \brief	    get the plugin release type
   /// \return     the plugin release type
   //--------------------------------------------------------------
   virtual const EReleaseType getReleaseType() const = 0;

   //--------------------------------------------------------------
   /// \brief	    get the plugin author
   /// \return     the plugin author
   //--------------------------------------------------------------
   virtual const std::string& getAuthor() const = 0;

   //--------------------------------------------------------------
   /// \brief	    get the plugin URL
   /// \return     the plugin URL
   //--------------------------------------------------------------
   virtual const std::string& getUrl() const = 0;

   //--------------------------------------------------------------
   /// \brief	    get all informations in printable format
   /// \return     all plugin informations
   //--------------------------------------------------------------
   virtual std::string toString() const = 0;
};

