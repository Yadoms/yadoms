#pragma once

#include <shared/Export.h>
#include "IInformation.h"

namespace shared { namespace plugin
{
   //--------------------------------------------------------------
   /// \brief		Container class for information about Hardware plugins
   /// \note      This class is usually implemented by IMPLEMENT_HARDWARE_PLUGIN macro
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CInformation : public IInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param      name : plugin name
      /// \param      version : plugin version
      /// \param      releaseType : plugin state
      /// \param      author : plugin author
      /// \param      url : plugin url
      /// \note      Version : Note that this value must be string-comparable to let Yadoms
      ///            know if newer version is avalaible (for the comparaison
      ///            method, see http://www.cplusplus.com/reference/string/string/compare/).
      ///            Mainly, don't change the version format in the plugin life.
      ///            Recommended format is "xx.yy"
      //--------------------------------------------------------------
      CInformation (const std::string & name, const std::string & version,
         const EReleaseType & releaseType, const std::string & author,
         const std::string & url);

      //--------------------------------------------------------------
      /// \brief	    Copy constructor
      /// \param      src : source
      //--------------------------------------------------------------
      CInformation (const IInformation& src);

      // shared::plugin::IInformation implementation
      virtual ~CInformation ();
      virtual const std::string& getName() const;
      virtual const std::string& getVersion() const;
      virtual const EReleaseType getReleaseType() const;
      virtual const std::string& getAuthor() const;
      virtual const std::string& getUrl() const;
      virtual std::string toString() const;
      // [END] shared::plugin::IInformation implementation

   private:
      std::string m_name;
      std::string m_version;
      EReleaseType m_releaseType;
      std::string m_author;
      std::string m_url;
   };
} } // namespace shared::plugin
