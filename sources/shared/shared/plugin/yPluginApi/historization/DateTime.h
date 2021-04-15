#pragma once
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
            ///\brief A dataTime historizable object
            //-----------------------------------------------------
            class CDateTime : public IHistorizable
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor
               ///\param[in] keywordName     Yadoms keyword name
               ///\param[in] accessMode      The access mode
               ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
               ///\param[in] historyDepth    The history depth policy
               //-----------------------------------------------------
               explicit CDateTime(std::string keywordName,
                                  const EKeywordAccessMode& accessMode = EKeywordAccessMode::kGet,
                                  const EMeasureType& measureType = EMeasureType::kCumulative,
                                  const EHistoryDepth& historyDepth = EHistoryDepth::kDefault);

               virtual ~CDateTime() = default;

               void set(const boost::posix_time::ptime& data);
               void setFromUnixTime(int64_t unixTime);
               void setFromIso(const std::string& isoDateTime);

               // IHistorizable implementation
               const std::string& getKeyword() const override;
               const CStandardCapacity& getCapacity() const override;
               const EKeywordAccessMode& getAccessMode() const override;
               std::string formatValue() const override;
               const EMeasureType& getMeasureType() const override;
               boost::shared_ptr<CDataContainer> getTypeInfo() const override;
               const EHistoryDepth& getHistoryDepth() const override;
               // [END] IHistorizable implementation

            private:
               boost::posix_time::ptime m_data;
               const std::string m_keywordName;
               const EKeywordAccessMode m_accessMode;
               const EMeasureType m_measureType;
               const EHistoryDepth m_historyDepth;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
