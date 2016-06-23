#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The command generic historizer
   //-----------------------------------------------------
   class CCommandHistorizer : public shared::plugin::yPluginApi::historization::IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Keyword name
      //-----------------------------------------------------
      explicit CCommandHistorizer(const std::string& keywordName, const std::string& value);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCommandHistorizer();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const;
      virtual const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const;
      virtual std::string formatValue() const;
      virtual const shared::plugin::yPluginApi::historization::EMeasureType& getMeasureType() const;
      virtual shared::CDataContainer getTypeInfo() const;
      // [END] IHistorizable implementation

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The value
      //-----------------------------------------------------
      std::string m_value;
   };

} // namespace pluginSystem	
	
	