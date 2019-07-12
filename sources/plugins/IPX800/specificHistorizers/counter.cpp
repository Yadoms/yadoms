#include "stdafx.h"
#include "counter.h"

namespace specificHistorizers
{
   CCounter::CCounter(const std::string& keywordName,
                      const std::string& hardwareName,
                      const yApi::EKeywordAccessMode& accessMode,
                      const yApi::EMeasureType& measureType,
                      const yApi::typeInfo::ITypeInfo& additionalInfo)
      : CSingleHistorizableData<Poco::Int64>(keywordName,
                                             yApi::CStandardCapacities::Counter(),
                                             accessMode,
                                             0,
                                             measureType,
                                             additionalInfo),
        m_hardwareName(hardwareName)
   {
   }

   std::string CCounter::getHardwareName() const
   {
      return m_hardwareName;
   }
} // namespace specificHistorizers
