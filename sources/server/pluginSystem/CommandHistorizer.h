#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include "database/common/requesters/Keyword.h"

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
      ///\param[in] keyword      Keyword
      ///\param[in] value        Command value
      //-----------------------------------------------------
      explicit CCommandHistorizer(boost::shared_ptr<const database::entities::CKeyword> keyword,
                                  const std::string& value);

      virtual ~CCommandHistorizer() = default;

      // IHistorizable implementation
      const std::string& getKeyword() const override;
      const shared::plugin::yPluginApi::CStandardCapacity& getCapacity() const override;
      const shared::plugin::yPluginApi::EKeywordAccessMode& getAccessMode() const override;
      std::string formatValue() const override;
      const shared::plugin::yPluginApi::EMeasureType& getMeasureType() const override;
      boost::shared_ptr<shared::CDataContainer> getTypeInfo() const override;
      const shared::plugin::yPluginApi::EHistoryDepth& getHistoryDepth() const override;
      // [END] IHistorizable implementation

   private:
      boost::shared_ptr<const database::entities::CKeyword> m_keyword;
      std::string m_value;
   };
} // namespace pluginSystem	
