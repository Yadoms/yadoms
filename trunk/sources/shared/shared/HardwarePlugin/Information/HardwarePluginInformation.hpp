#pragma once

#include <string>

#include "../Interfaces/IHardwarePluginInformation.h"


//TODO : faire un CPP ?

//--------------------------------------------------------------
/// \brief		Container class for information about Hardware plugins
/// \note      This class is usually implemented by IMPLEMENT_HARDWARE_PLUGIN macro
//--------------------------------------------------------------
class CHardwarePluginInformation : public IHardwarePluginInformation
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
   CHardwarePluginInformation (const std::string & name , const std::string & version,
      const EReleaseType & releaseType, const std::string & author,
      const std::string & url)
      :m_name(name), m_version(version), m_releaseType(releaseType), m_author(author), m_url(url)
   {
   }

   //--------------------------------------------------------------
   /// \brief	    Copy constructor
   /// \param      src : source
   //--------------------------------------------------------------
   CHardwarePluginInformation (const IHardwarePluginInformation& src)
      :m_name(src.getName()), m_version(src.getVersion()), m_releaseType(src.getReleaseType()), m_author(src.getAuthor()), m_url(src.getUrl())
   {
   }

   // IHardwarePluginInformation implementation
   virtual ~CHardwarePluginInformation () { }
   virtual const std::string& getName() const { return  m_name; }
   virtual const std::string& getVersion() const { return  m_version; }
   virtual const EReleaseType getReleaseType() const { return m_releaseType; }
   virtual const std::string& getAuthor() const { return  m_author; }
   virtual const std::string& getUrl() const { return  m_url; }
   virtual std::string toString() const
   {
      std::ostringstream formatedInformations;

      formatedInformations << m_name;
      formatedInformations << " v" << m_version;
      formatedInformations << "[" << m_releaseType << "]";
      formatedInformations << " by " << m_author;
      formatedInformations << " (" << m_url << ")";

      return formatedInformations.str();
   }
   // [END] IHardwarePluginInformation implementation

private:
   std::string m_name;
   std::string m_version;
   EReleaseType m_releaseType;
   std::string m_author;
   std::string m_url;
};


