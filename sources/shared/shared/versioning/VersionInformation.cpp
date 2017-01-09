#include "stdafx.h"
#include "VersionInformation.h"
#include <shared/Log.h>

namespace shared { namespace versioning {

   CVersionInformation::CVersionInformation(const shared::versioning::CVersion & version)
   {
      m_container.set("version", version.toString());
   }

   const shared::versioning::CVersion CVersionInformation::getVersion() const
   {
      return shared::versioning::CVersion(m_container.get<std::string>("version"));
   }

   const std::string CVersionInformation::serialize() const
   {
      return m_container.serialize();
   }

   const shared::CDataContainer & CVersionInformation::getContainer() const
   {
      return m_container;
   }


} } // namespace shared::versioning
