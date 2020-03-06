#pragma once
#include <shared/plugin/yPluginApi/typeInfo/ITypeInfo.h>
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
      class CCustomEnumTypeInfo : public yApi::typeInfo::ITypeInfo
      {
      public:
         explicit CCustomEnumTypeInfo(const std::string& typeName,
                                      const std::vector<std::string>& values);
         virtual ~CCustomEnumTypeInfo();

         // yApi::typeInfo::ITypeInfo implementation 
         shared::CDataContainerSharedPtr serialize() const override;
         // END yApi::typeInfo::ITypeInfo implementation 

         shared::CDataContainerSharedPtr m_data;
      };
   }
} // namespace pluginSystem::internalPlugin


