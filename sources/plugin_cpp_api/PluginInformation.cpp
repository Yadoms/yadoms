#include "stdafx.h"
#include "PluginInformation.h"


namespace plugin_cpp_api
{
   CPluginInformation::CPluginInformation(boost::shared_ptr<const plugin_IPC::toPlugin::Information> buffer)
      : m_buffer(std::move(buffer)),
        m_path(m_buffer->path()),
        m_version(m_buffer->version())
   {
   }

   const std::string& CPluginInformation::getType() const
   {
      return m_buffer->type();
   }

   const shared::versioning::CSemVer& CPluginInformation::getVersion() const
   {
      return m_version;
   }

   const std::string& CPluginInformation::getAuthor() const
   {
      return m_buffer->author();
   }

   const std::string& CPluginInformation::getUrl() const
   {
      return m_buffer->url();
   }

   std::string CPluginInformation::getIdentity() const
   {
      return m_buffer->identity();
   }

   std::string CPluginInformation::toString() const
   {
      return m_buffer->tostring();
   }

   bool CPluginInformation::isSupportedOnThisPlatform() const
   {
      return m_buffer->supportedonthisplatform();
   }

   bool CPluginInformation::getSupportManuallyCreatedDevice() const
   {
      return m_buffer->supportmanuallycreateddevice();
   }

   bool CPluginInformation::getSupportDeviceRemovedNotification() const
   {
      return m_buffer->supportdeviceremovednotification();
   }

   boost::shared_ptr<const shared::CDataContainer> CPluginInformation::getConfigurationSchema() const
   {
      const auto package = getPackage();
      if (package->containsChild("configurationSchema"))
         return getPackage()->getChild("configurationSchema");
      return shared::CDataContainer::EmptyContainerSharedPtr;
   }

   boost::shared_ptr<const shared::CDataContainer> CPluginInformation::getDeviceStaticConfigurationSchema() const
   {
      const auto package = getPackage();
      if (package->containsChild("deviceConfiguration.staticConfigurationSchema"))
         return getPackage()->getChild("deviceConfiguration.staticConfigurationSchema");
      return nullptr;
   }

   boost::shared_ptr<const shared::CDataContainer> CPluginInformation::getDeviceDynamicConfigurationSchema() const
   {
      const auto package = getPackage();
      if (package->containsChild("deviceConfiguration.dynamicConfigurationSchema"))
         return getPackage()->getChild("deviceConfiguration.dynamicConfigurationSchema");
      return nullptr;
   }

   boost::shared_ptr<const shared::CDataContainer> CPluginInformation::getLabels(const std::vector<std::string>& locales) const
   {
      throw std::runtime_error("Should not be used");
   }

   boost::shared_ptr<const shared::CDataContainer> CPluginInformation::getPackage() const
   {
      return boost::make_shared<const shared::CDataContainer>(m_buffer->packagefilecontent());
   }

   const boost::filesystem::path& CPluginInformation::getPath() const
   {
      return m_path;
   }
} // namespace plugin_cpp_api	
