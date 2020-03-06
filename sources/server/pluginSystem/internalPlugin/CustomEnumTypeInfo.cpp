#include "stdafx.h"
#include "CustomEnumTypeInfo.h"

namespace pluginSystem
{
   namespace internalPlugin
   {
      CCustomEnumTypeInfo::CCustomEnumTypeInfo(const std::string& typeName,
                                               const std::vector<std::string>& values)
         :m_data(new_CDataContainerSharedPtr())
      {
         m_data->set("name", typeName);
         if (values.empty())
            throw std::invalid_argument("Custom enum must have values");
         m_data->set("values", values);
      }

      CCustomEnumTypeInfo::~CCustomEnumTypeInfo()
      {
      }

      shared::CDataContainerSharedPtr CCustomEnumTypeInfo::serialize() const
      {
         return m_data;
      }
   }
} // namespace pluginSystem::internalPlugin


