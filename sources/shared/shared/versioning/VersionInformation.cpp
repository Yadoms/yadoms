#include "stdafx.h"
#include "VersionInformation.h"
#include <shared/Log.h>

namespace shared
{
   namespace versioning
   {
      CVersionInformation::CVersionInformation(const CSemVer& version)
      {
         m_container.set("version", version.toString());
      }

      CSemVer CVersionInformation::getVersion() const
      {
         return CSemVer(m_container.get<std::string>("version"));
      }

      std::string CVersionInformation::serialize() const
      {
         return m_container.serialize();
      }

      const CDataContainer& CVersionInformation::getContainer() const
      {
         return m_container;
      }
   }
} // namespace shared::versioning
