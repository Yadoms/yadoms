#pragma once

#include <string>
#include "ReleaseType.h"

namespace shared { namespace plugin { namespace information
{
   //--------------------------------------------------------------
   /// \brief		Interface for information about plugins
   //--------------------------------------------------------------
   class IInformation 
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
      /// \brief	    get all identity informations in printable format
      /// \return     all plugin informations (plugin name and version)
      //--------------------------------------------------------------
      virtual std::string getIdentity() const = 0;

      //--------------------------------------------------------------
      /// \brief	    get all informations in printable format
      /// \return     all plugin informations
      //--------------------------------------------------------------
      virtual std::string toString() const = 0;


      //--------------------------------------------------------------
      /// \brief	    Check if plugin is supported on this platform
      /// \return     true if supported, false if not
      //--------------------------------------------------------------
      virtual bool isSupportedOnThisPlatform() const = 0;


      //--------------------------------------------------------------
      /// \brief	    indicates if the plugin supports manually created devices
      /// \return     true if the plugin supports manually created devices
      //--------------------------------------------------------------
      virtual bool getSupportManuallyCreatedDevice() const = 0;
   };
} } } // namespace shared::plugin::information
