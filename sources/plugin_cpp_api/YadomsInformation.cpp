#include "stdafx.h"
#include "YadomsInformation.h"
#include "Location.h"


namespace plugin_cpp_api
{
   CYadomsInformation::CYadomsInformation(const plugin_IPC::toPlugin::YadomsInformationAnswer& buffer)
      : m_buffer(buffer)
   {
   }

   CYadomsInformation::~CYadomsInformation()
   {
   }

   bool CYadomsInformation::developperMode() const
   {
      return m_buffer.developpermode();
   }

   std::string CYadomsInformation::version() const
   {
      return m_buffer.version();
   }

   shared::versioning::EReleaseType CYadomsInformation::releaseType() const
   {
      switch (m_buffer.releasetype())
      {
      case plugin_IPC::toPlugin::YadomsInformationAnswer_EReleaseType_Stable: return shared::versioning::EReleaseType::kStable;
      case plugin_IPC::toPlugin::YadomsInformationAnswer_EReleaseType_ReleaseCandidate: return shared::versioning::EReleaseType::kReleaseCandidate;
      case plugin_IPC::toPlugin::YadomsInformationAnswer_EReleaseType_Beta: return shared::versioning::EReleaseType::kBeta;
      default: return shared::versioning::EReleaseType::kBeta;
      }
   }

   boost::shared_ptr<const shared::ILocation> CYadomsInformation::location() const
   {
      if (m_buffer.has_location())
         return boost::make_shared<CLocation>(m_buffer.location());
      return boost::shared_ptr<const shared::ILocation>();
   }
} // namespace plugin_cpp_api	


