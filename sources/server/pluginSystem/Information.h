#pragma once

#include <shared/plugin/information/IInformation.h>
#include <shared/DataContainer.h>

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief		Container class for plugin information
   //--------------------------------------------------------------
   class CInformation : public shared::plugin::information::IInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    pluginPath : plugin path
      /// \throw        exception::CInvalidParameter if file is not valid or complete
      //--------------------------------------------------------------
      explicit CInformation(const boost::filesystem::path& pluginPath);

      virtual ~CInformation();

      // shared::plugin::IInformation implementation
      const std::string& getType() const override;
      const shared::versioning::CSemVer& getVersion() const override;
      const std::string& getAuthor() const override;
      const std::string& getUrl() const override;
      std::string getIdentity() const override;
      std::string toString() const override;
      bool isSupportedOnThisPlatform() const override;
      bool getSupportManuallyCreatedDevice() const override;
      bool getSupportDeviceRemovedNotification() const override;
      boost::shared_ptr<const shared::CDataContainer> getPackage() const override;
      const boost::filesystem::path& getPath() const override;
      // [END] shared::plugin::IInformation implementation

   private:
      //--------------------------------------------------------------
      /// \brief	    plugin type
      //--------------------------------------------------------------
      std::string m_type;

      //--------------------------------------------------------------
      /// \brief	    Version
      /// \note       Version : Note that this value must be string-comparable to let Yadoms
      ///             know if newer version is available (for the comparison
      ///             method, see http://www.cplusplus.com/reference/string/string/compare/).
      ///             Mainly, don't change the version format in the plugin life.
      ///             Recommended format is "xx.yy"
      //--------------------------------------------------------------
      shared::versioning::CSemVer m_version;

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

      //--------------------------------------------------------------
      /// \brief	    true if the plugin supports manually created devices
      //--------------------------------------------------------------
      bool m_supportManuallyCreatedDevice;

      //--------------------------------------------------------------
      /// \brief	    true if the plugin supports device removed notification
      //--------------------------------------------------------------
      bool m_supportDeviceRemovedNotification;
      
      //--------------------------------------------------------------
      /// \brief	    Flag indicating if plugin is supported on this platform
      //--------------------------------------------------------------
      bool m_isSupportedOnThisPlatform;

      //--------------------------------------------------------------
      /// \brief	    Package.json content
      //--------------------------------------------------------------
      boost::shared_ptr<shared::CDataContainer> m_package;
   };

} // namespace pluginSystem
