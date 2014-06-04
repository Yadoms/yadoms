#pragma once

#include <shared/plugin/information/IInformation.h>

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief		Container class for information about plugins
   /// \note      This class is usually implemented by IMPLEMENT_PLUGIN macro
   //--------------------------------------------------------------
   class CInformation : public shared::plugin::information::IInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    pluginPath : plugin executing path
      /// \throw        exception::CInvalidParameter if file is not valid or complete
      //--------------------------------------------------------------
      CInformation(const boost::filesystem::path& pluginPath);

      // shared::plugin::IInformation implementation
      virtual ~CInformation();
      virtual const std::string& getName() const;
      virtual const std::string& getDescription() const;
      virtual const std::string& getVersion() const;
      virtual const shared::plugin::information::EReleaseType getReleaseType() const;
      virtual const std::string& getAuthor() const;
      virtual const std::string& getUrl() const;
      virtual std::string toString() const;
      virtual std::string getIdentity() const;
      // [END] shared::plugin::IInformation implementation

      //--------------------------------------------------------------
      /// \brief	    Get plugin path
      /// \return     The plugin executing path
      //--------------------------------------------------------------
      const boost::filesystem::path getPath() const;

   private:
      //--------------------------------------------------------------
      /// \brief	    plugin name
      //--------------------------------------------------------------
      std::string m_name;

      //--------------------------------------------------------------
      /// \brief      plugin description (i18n multiline string)
      //--------------------------------------------------------------
      std::string m_description;

      //--------------------------------------------------------------
      /// \brief	    Version
      /// \note       Version : Note that this value must be string-comparable to let Yadoms
      ///             know if newer version is available (for the comparison
      ///             method, see http://www.cplusplus.com/reference/string/string/compare/).
      ///             Mainly, don't change the version format in the plugin life.
      ///             Recommended format is "xx.yy"
      //--------------------------------------------------------------
      std::string m_version;

      //--------------------------------------------------------------
      /// \brief      plugin state
      //--------------------------------------------------------------
      shared::plugin::information::EReleaseType m_releaseType;

      //--------------------------------------------------------------
      /// \brief      plugin author
      //--------------------------------------------------------------
      std::string m_author;

      //--------------------------------------------------------------
      /// \brief      plugin url
      //--------------------------------------------------------------
      std::string m_url;

      //--------------------------------------------------------------
      /// \brief	    Plugin path
      //--------------------------------------------------------------
      const boost::filesystem::path m_path;
   };

} // namespace pluginSystem
