#include "stdafx.h"
#include "counter.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace specificHistorizers
{
	CCounter::CCounter(const std::string& keywordName,
	                   const std::string& hardwareName,
					   const yApi::EKeywordAccessMode& accessMode,
					   const yApi::historization::EMeasureType& measureType,
                      yApi::historization::typeInfo::ITypeInfo& additionalInfo)
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
   
	CCounter::~CCounter()
	{}   

} // namespace specificHistorizers

