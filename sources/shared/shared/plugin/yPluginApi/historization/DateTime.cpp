#include "stdafx.h"
#include "DateTime.h"

#include <utility>
#include "../../../currentTime/Provider.h"
#include "../StandardCapacities.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            CDateTime::CDateTime(std::string keywordName,
                                 const EKeywordAccessMode& accessMode,
                                 const EMeasureType& measureType,
                                 const EHistoryDepth& historyDepth)
               : m_keywordName(std::move(keywordName)),
                 m_accessMode(accessMode),
                 m_measureType(measureType),
                 m_historyDepth(historyDepth)
            {
            }

            const std::string& CDateTime::getKeyword() const
            {
               return m_keywordName;
            }

            const CStandardCapacity& CDateTime::getCapacity() const
            {
               return CStandardCapacities::DateTime();
            }

            const EKeywordAccessMode& CDateTime::getAccessMode() const
            {
               return m_accessMode;
            }

            void CDateTime::set(const boost::posix_time::ptime& data)
            {
               m_data = data;
            }

            void CDateTime::setFromUnixTime(int64_t unixTime)
            {
               m_data = boost::posix_time::from_time_t(unixTime);
            }

            void CDateTime::setFromIso(const std::string& isoDateTime)
            {
               m_data = boost::posix_time::time_from_string(isoDateTime);
            }

            std::string CDateTime::formatValue() const
            {
               return to_iso_string(m_data);
            }

            const EMeasureType& CDateTime::getMeasureType() const
            {
               return m_measureType;
            }

            boost::shared_ptr<CDataContainer> CDateTime::getTypeInfo() const
            {
               return shared::CDataContainer::make();
            }

            const EHistoryDepth& CDateTime::getHistoryDepth() const
            {
               return m_historyDepth;
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
