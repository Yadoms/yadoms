#include "stdafx.h"
#include "MoonHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   const CStandardCapacity& MoonCapacity = CStandardCapacity("Moon", CStandardUnits::NoUnits, EKeywordDataType::kNoData);

   
   CMoonHistorizer::CMoonHistorizer(const std::string& keywordName, 
                                              const EKeywordAccessMode& accessMode 
                                              )
   :m_keywordName(keywordName), m_accessMode(accessMode)
   {
	   m_content.reset(new CMoonFormatter( ));
   }

   CMoonHistorizer::~CMoonHistorizer()
   {}

	const std::string& CMoonHistorizer::getKeyword() const
	{
	   return m_keywordName;
	}

	const CStandardCapacity& CMoonHistorizer::getCapacity() const
	{
	   return MoonCapacity;
	}
	
	const EKeywordAccessMode& CMoonHistorizer::getAccessMode() const
	{
	   return m_accessMode;
	}

void CMoonHistorizer::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
   m_content->AddUnit ( UnitName, UnitValue );
}

	void CMoonHistorizer::SetParameters(
            const std::string& IlluminatedMoon,
            const std::string& DayofMoon
				)
	{
		m_content->SetParameters( IlluminatedMoon, DayofMoon );
   }

	std::string CMoonHistorizer::formatValue() const
	{
	   return !m_content ? std::string() : m_content->formatValue();
	}

	const EMeasureType& CMoonHistorizer::getMeasureType() const
	{
	   static const EMeasureType MeasureType(EMeasureType::kAbsolute);
	   return MeasureType;
	}	

   CDataContainer CMoonHistorizer::getTypeInfo() const
   {
      return CDataContainer();
   }

} } } } // namespace shared::plugin::yPluginApi::historization