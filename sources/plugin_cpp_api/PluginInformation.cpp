#include "stdafx.h"
#include "PluginInformation.h"


namespace plugin_cpp_api
{
   CPluginInformation::CPluginInformation(boost::shared_ptr<const plugin_IPC::toPlugin::Information> buffer)
      : m_buffer(buffer),
        m_path(m_buffer->path())
   {
   }

   CPluginInformation::~CPluginInformation()
   {
   }

   const std::string& CPluginInformation::getType() const
   {
      return m_buffer->type();
   }

   const std::string& CPluginInformation::getVersion() const
   {
      return m_buffer->version();
   }

   shared::versioning::EReleaseType CPluginInformation::getReleaseType() const
   {
      switch (m_buffer->releasetype())
      {
      case plugin_IPC::toPlugin::Information_EReleaseType_kStable: return shared::versioning::EReleaseType::kStable;
      case plugin_IPC::toPlugin::Information_EReleaseType_kReleaseCandidate: return shared::versioning::EReleaseType::kReleaseCandidate;
      default: return shared::versioning::EReleaseType::kBeta;
      }
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


