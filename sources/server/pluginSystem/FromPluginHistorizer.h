#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <plugin_IPC/pluginToYadoms.pb.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The command generic historizer
   //-----------------------------------------------------
   class CFromPluginHistorizer : public shared::plugin::yPluginApi::historization::IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] historizable    Historizable data from Protobuf buffer
      ///\param[in] formatValue     Value (for historization, not need for keyword declaration)
      //-----------------------------------------------------
      explicit CFromPluginHistorizer(const plugin_IPC::toYadoms::Historizable& historizable,
                                     const std::string& formatValue = std::string());

      virtual ~CFromPluginHistorizer() = default;

      // IHistorizable implementation
      const std::string& getKeyword() const override;
      const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const override;
      const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const override;
      std::string formatValue() const override;
      const shared::plugin::yPluginApi::EMeasureType& getMeasureType() const override;
      shared::CDataContainerSharedPtr getTypeInfo() const override;
      const shared::plugin::yPluginApi::EHistoryDepth& getHistoryDepth() const override;
      // [END] IHistorizable implementation

   private:
      const std::string m_keyword;
      const shared::plugin::yPluginApi::CStandardCapacity m_capacity;
      const shared::plugin::yPluginApi::EKeywordAccessMode m_accessMode;
      const shared::plugin::yPluginApi::EMeasureType m_measureType;
      const shared::CDataContainerSharedPtr m_typeInfo;
      const shared::plugin::yPluginApi::EHistoryDepth m_historyDepth;
      const std::string m_value;
   };
} // namespace pluginSystem	
