#include "stdafx.h"
#include "CustomEnumTypeInfo.h"

namespace pluginSystem
{
   namespace internalPlugin
   {
      CCustomEnumTypeInfo::CCustomEnumTypeInfo(const std::string& typeName,
                                               const std::vector<std::string>& values)
      {
         m_data.set("name", typeName);
         m_data.set("values", values);
      }

      CCustomEnumTypeInfo::~CCustomEnumTypeInfo()
      {
      }

      shared::CDataContainer CCustomEnumTypeInfo::serialize() const
      {
         return m_data;
      }
   }
} // namespace pluginSystem::internalPlugin


