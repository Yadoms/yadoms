#pragma once
#include <shared/plugin/yPluginApi/historization/typeInfo/ITypeInfo.h>
#include <shared/DataContainer.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace pluginSystem
{
   namespace internalPlugin
   {
      //-----------------------------------------------------
      ///\brief Interface for keyword type information
      //-----------------------------------------------------
      class CCustomEnumTypeInfo : public yApi::historization::typeInfo::ITypeInfo
      {
      public:
         explicit CCustomEnumTypeInfo(const std::string& typeName,
                                      const std::vector<std::string>& values);
         virtual ~CCustomEnumTypeInfo();

         // yApi::historization::typeInfo::ITypeInfo implementation 
         shared::CDataContainer serialize() const override;
         // END yApi::historization::typeInfo::ITypeInfo implementation 

         shared::CDataContainer m_data;
      };
   }
} // namespace pluginSystem::internalPlugin


