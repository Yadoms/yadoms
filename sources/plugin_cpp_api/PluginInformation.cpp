#include "stdafx.h"
#include "PluginInformation.h"


namespace plugin_cpp_api
{
   CPluginInformation::CPluginInformation(boost::shared_ptr<const plugin_IPC::toPlugin::Information> buffer)
      : m_buffer(buffer),
        m_path(m_buffer->path()),
        m_version(m_buffer->version())
   {
   }

   CPluginInformation::~CPluginInformation()
   {
   }

   const std::string& CPluginInformation::getType() const
   {
      return m_buffer->type();
   }

   const shared::versioning::CVersion& CPluginInformation::getVersion() const
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

   boost::shared_ptr<const shared::CDataContainer> CPluginInformation::getPackage() const
   {
      return boost::make_shared<const shared::CDataContainer>(m_buffer->packagefilecontent());
   }

   const boost::filesystem::path& CPluginInformation::getPath() const
   {
      return m_path;
   }
} // namespace plugin_cpp_api	


