#include "stdafx.h"
#include "YadomsInformation.h"
#include "Location.h"


namespace plugin_cpp_api
{
   CYadomsInformation::CYadomsInformation(const toPlugin::YadomsInformationAnswer& buffer)
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

   shared::versioning::CVersion CYadomsInformation::version() const
   {
      return shared::versioning::CVersion(m_buffer.version());
   }

   boost::shared_ptr<const shared::ILocation> CYadomsInformation::location() const
   {
      return boost::make_shared<CLocation>(m_buffer.location());
   }
} // namespace plugin_cpp_api	


