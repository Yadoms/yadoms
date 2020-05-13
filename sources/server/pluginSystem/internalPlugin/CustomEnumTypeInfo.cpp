#include "stdafx.h"
#include "CustomEnumTypeInfo.h"

namespace pluginSystem
{
   namespace internalPlugin
   {
      CCustomEnumTypeInfo::CCustomEnumTypeInfo(const std::string& typeName,
                                               const std::vector<std::string>& values)
         :m_data(shared::CDataContainer::make())
      {
         m_data->set("name", typeName);
         if (values.empty())
            throw std::invalid_argument("Custom enum must have values");
         m_data->set("values", values);
      }

      CCustomEnumTypeInfo::~CCustomEnumTypeInfo()
      {
      }

      boost::shared_ptr<shared::CDataContainer> CCustomEnumTypeInfo::serialize() const
      {
         return m_data;
      }
   }
} // namespace pluginSystem::internalPlugin


