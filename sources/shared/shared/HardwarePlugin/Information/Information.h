#pragma once

#include <shared/Export.h>
#include "../Interfaces/IHardwarePluginInformation.h"


//--------------------------------------------------------------
/// \brief		Container class for information about Hardware plugins
/// \note      This class is usually implemented by IMPLEMENT_HARDWARE_PLUGIN macro
//--------------------------------------------------------------
class YADOMS_SHARED_EXPORT CHardwarePluginInformation : public IHardwarePluginInformation
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
   CHardwarePluginInformation (const std::string & name, const std::string & version,
      const EReleaseType & releaseType, const std::string & author,
      const std::string & url);

   //--------------------------------------------------------------
   /// \brief	    Copy constructor
   /// \param      src : source
   //--------------------------------------------------------------
   CHardwarePluginInformation (const IHardwarePluginInformation& src);

   // IHardwarePluginInformation implementation
   virtual ~CHardwarePluginInformation ();
   virtual const std::string& getName() const;
   virtual const std::string& getVersion() const;
   virtual const EReleaseType getReleaseType() const;
   virtual const std::string& getAuthor() const;
   virtual const std::string& getUrl() const;
   virtual std::string toString() const;
   // [END] IHardwarePluginInformation implementation

private:
   std::string m_name;
   std::string m_version;
   EReleaseType m_releaseType;
   std::string m_author;
   std::string m_url;
};


