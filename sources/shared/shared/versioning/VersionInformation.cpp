#include "stdafx.h"
#include "VersionInformation.h"
#include <shared/Log.h>

namespace shared { namespace versioning {

   CVersionInformation::CVersionInformation(const shared::versioning::CVersion & version, const shared::versioning::EReleaseType & releaseType)
   {
      m_container.set("version", version.toString(3));
      m_container.set("releaseType", releaseType);
   }

   const shared::versioning::CVersion CVersionInformation::getVersion() const
   {
      return shared::versioning::CVersion(m_container.get<std::string>("version"));
   }

   const shared::versioning::EReleaseType CVersionInformation::getReleaseType() const
   {
      return m_container.get<shared::versioning::EReleaseType>("releaseType");
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
