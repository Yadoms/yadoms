#pragma once

#include <string>

//--------------------------------------------------------------
/// \brief		Container class for information about Hardware plugins
/// \note      This class is usually implemented by IMPLEMENT_HARDWARE_PLUGIN macro
//--------------------------------------------------------------
class CHardwarePluginInformation 
{
public:
   //--------------------------------------------------------------
   /// \brief		Enum for plugin state
   //--------------------------------------------------------------
   enum EReleaseType
   {
      kStable,    /// \brief		plugin is stable
      kTesting,   /// \brief		plugin is in testing phase
      kBeta       /// \brief		plugin is in beta state
   };


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
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginInformation ()
   {
   }

public:
   //--------------------------------------------------------------
   /// \brief	    get the plugin Name
   /// \return     the plugin name
   //--------------------------------------------------------------
   const std::string& getName() const
   { 
      return  m_name; 
   }

   //--------------------------------------------------------------
   /// \brief	    get the plugin version
   /// \return     the plugin version
   //--------------------------------------------------------------
   const std::string& getVersion() const
   { 
      return  m_version; 
   }

   //--------------------------------------------------------------
   /// \brief	    get the plugin release type
   /// \return     the plugin release type
   //--------------------------------------------------------------
   const EReleaseType getReleaseType() const
   { 
      return m_releaseType; 
   }

   //--------------------------------------------------------------
   /// \brief	    get the plugin author
   /// \return     the plugin author
   //--------------------------------------------------------------
   const std::string& getAuthor() const
   { 
      return  m_author; 
   }

   //--------------------------------------------------------------
   /// \brief	    get the plugin URL
   /// \return     the plugin URL
   //--------------------------------------------------------------
   const std::string& getUrl() const
   { 
      return  m_url; 
   }

private:
   std::string m_name;
   std::string m_version;
   EReleaseType m_releaseType;
   std::string m_author;
   std::string m_url;
};


