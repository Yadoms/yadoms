#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include "CustomEnumTypeInfo.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace pluginSystem
{
   namespace internalPlugin
   {
      //-----------------------------------------------------
      ///\brief A custom enum historizer
      //-----------------------------------------------------
      class CCustomEnumHistorizer : public yApi::historization::IHistorizable
      {
      public:
         CCustomEnumHistorizer(const std::string& keywordName,
                               const yApi::EKeywordAccessMode& keywordAccessMode,
                               const std::vector<std::string>& values);
         virtual ~CCustomEnumHistorizer() = default;

         // yApi::historization::IHistorizable Implementation
         const std::string& getKeyword() const override;
         const yApi::CStandardCapacity& getCapacity() const override;
         const yApi::EKeywordAccessMode& getAccessMode() const override;
         std::string formatValue() const override;
         const yApi::EMeasureType& getMeasureType() const override;
         boost::shared_ptr<shared::CDataContainer> getTypeInfo() const override;
         const yApi::EHistoryDepth& getHistoryDepth() const override;
         // [END]yApi::historization::IHistorizable Implementation

      private:
         const std::string m_keywordName;
         const yApi::EKeywordAccessMode m_keywordAccessMode;
         const CCustomEnumTypeInfo m_typeInfo;

         std::string m_value;
      };
   }
} // namespace pluginSystem::internalPlugin


