#pragma once

#include <shared/Export.h>
#include <string>

namespace shared { namespace plugin { namespace information
{
   //--------------------------------------------------------------
   /// \brief		Enum for plugin release state
   //--------------------------------------------------------------
   enum EReleaseType
   {
      kStable = 0,   /// \brief		plugin is stable
      kTesting,      /// \brief		plugin is in testing phase
      kBeta          /// \brief		plugin is in beta state
   };

   //--------------------------------------------------------------
   /// \brief		Interface for information about plugins
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT IInformation 
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IInformation () { }

      //--------------------------------------------------------------
      /// \brief	    get the plugin Name
      /// \return     the plugin name
      //--------------------------------------------------------------
      virtual const std::string& getName() const = 0;

      //--------------------------------------------------------------
      /// \brief	    get the plugin Description
      /// \return     the plugin description (can be multiline string)
      //--------------------------------------------------------------
      virtual const std::string& getDescription() const = 0;

      //--------------------------------------------------------------
      /// \brief	    get the plugin version
      /// \return     the plugin version
      /// \note       Version : Note that this value must be string-comparable to let Yadoms
      ///             know if newer version is available (for the comparison
      ///             method, see http://www.cplusplus.com/reference/string/string/compare/).
      ///             Mainly, don't change the version format in the plugin life.
      ///             Recommended format is "xx.yy"
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
} } } // namespace shared::plugin::information
