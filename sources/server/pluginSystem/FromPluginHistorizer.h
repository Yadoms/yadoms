#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <plugin_IPC/plugin_IPC.h>

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
      //-----------------------------------------------------
      explicit CFromPluginHistorizer(const toYadoms::Historizable& historizable);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CFromPluginHistorizer();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const;
      virtual const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const;
      virtual std::string formatValue() const;
      virtual const shared::plugin::yPluginApi::historization::EMeasureType& getMeasureType() const;
      virtual shared::CDataContainer getTypeInfo() const;
      // [END] IHistorizable implementation

   private:
      const std::string m_keyword;
      const shared::plugin::yPluginApi::EKeywordAccessMode m_accessMode;
      const shared::plugin::yPluginApi::historization::EMeasureType m_measureType;
      const shared::CDataContainer m_typeInfo;
      const std::string m_value;
   };

} // namespace pluginSystem	
	
	