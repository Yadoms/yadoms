#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include "IHistorizable.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief A event historizable object (no data)
            //-----------------------------------------------------
            class YADOMS_SHARED_EXPORT CEvent : public IHistorizable
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               //-----------------------------------------------------
               explicit CEvent(const std::string& keywordName,
                               const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGetSet);

               virtual ~CEvent() = default;

               // IHistorizable implementation
               const std::string& getKeyword() const override;
               const CStandardCapacity& getCapacity() const override;
               const EKeywordAccessMode& getAccessMode() const override;
               std::string formatValue() const override;
               const EMeasureType& getMeasureType() const override;
               CDataContainer getTypeInfo() const override;
               const EHistoryDepth& getHistoryDepth() const override;
               // [END] IHistorizable implementation

            private:
               //-----------------------------------------------------
               ///\brief                     The keyword name
               //-----------------------------------------------------
               const std::string m_keywordName;

               //-----------------------------------------------------
               ///\brief                     The access mode
               //-----------------------------------------------------
               const EKeywordAccessMode m_accessMode;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


