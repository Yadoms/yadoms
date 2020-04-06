#pragma once

#include <shared/plugin/information/IInformation.h>
#include <plugin_IPC/pluginInit.pb.h>

namespace plugin_cpp_api
{
   class CPluginInformation :public shared::plugin::information::IInformation
   {
   public:
      //--------------------------------------------------------------
      /// \brief	      Constructor
      /// \param[in]    buffer : Protobuf buffer
      //--------------------------------------------------------------
      explicit CPluginInformation(boost::shared_ptr<const plugin_IPC::toPlugin::Information> buffer);

      virtual ~CPluginInformation();

      // shared::plugin::information::IInformation implementation
      const std::string& getType() const override;
      const shared::versioning::CVersion& getVersion() const override;
      const std::string& getAuthor() const override;
      const std::string& getUrl() const override;
      std::string getIdentity() const override;
      std::string toString() const override;
      bool isSupportedOnThisPlatform() const override;
      bool getSupportManuallyCreatedDevice() const override;
      bool getSupportDeviceRemovedNotification() const override;
      boost::shared_ptr<const shared::CDataContainer> getPackage() const override;
      const boost::filesystem::path& getPath() const override;
      // [END] shared::plugin::information::IInformation implementation

   private:
      boost::shared_ptr<const plugin_IPC::toPlugin::Information> m_buffer;
      const boost::filesystem::path m_path;
      shared::versioning::CVersion m_version;
   };
} // namespace plugin_cpp_api	


