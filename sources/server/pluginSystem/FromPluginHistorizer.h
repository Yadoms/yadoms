#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <plugin_IPC/toYadoms.pb.h>

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
      CFromPluginHistorizer(const plugin_IPC::toYadoms::Historizable& historizable,
                            const std::string& formatValue = std::string());
      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CFromPluginHistorizer();

      // IHistorizable implementation
      const std::string& getKeyword() const override;
      const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const override;
      const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const override;
      std::string formatValue() const override;
      const shared::plugin::yPluginApi::historization::EMeasureType& getMeasureType() const override;
      shared::CDataContainer getTypeInfo() const override;
      // [END] IHistorizable implementation

   private:
      const std::string m_keyword;
      const shared::plugin::yPluginApi::CStandardCapacity m_capacity;
      const shared::plugin::yPluginApi::EKeywordAccessMode m_accessMode;
      const shared::plugin::yPluginApi::historization::EMeasureType m_measureType;
      const shared::CDataContainer m_typeInfo;
      const std::string m_value;
   };
} // namespace pluginSystem	


