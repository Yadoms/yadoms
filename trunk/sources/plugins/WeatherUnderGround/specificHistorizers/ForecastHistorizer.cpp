#include "stdafx.h"
#include "ForecastHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   const CStandardCapacity& ForecastCapacity = CStandardCapacity("Forecast", CStandardUnits::NoUnits, EKeywordDataType::kNoData);

   
   CForecastHistorizer::CForecastHistorizer(const std::string& keywordName, const EKeywordAccessMode& accessMode, const std::string & Period)
   :m_keywordName(keywordName), m_accessMode(accessMode)
   {
	   m_content.reset(new CForecastFormatter( Period ));
   }

   CForecastHistorizer::~CForecastHistorizer()
   {}

	const std::string& CForecastHistorizer::getKeyword() const
	{
	   return m_keywordName;
	}

	const CStandardCapacity& CForecastHistorizer::getCapacity() const
	{
	   return ForecastCapacity;
	}
	
	const EKeywordAccessMode& CForecastHistorizer::getAccessMode() const
	{
	   return m_accessMode;
	}

void CForecastHistorizer::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
   m_content->AddUnit ( UnitName, UnitValue );
}

	void CForecastHistorizer::AddPeriod(
                const std::string& Year,
                const std::string& Month,
                const std::string& Day,
		          const std::string& WeatherCondition, 
	             const std::string& TempMax, 
					 const std::string& TempMin,
					 const std::string& MaxWind,
					 const std::string& AveWind,
					 const std::string& AveHumidity,
                const std::string& RainDay
					 )
	{
		m_content->AddPeriod( Year, Month, Day, WeatherCondition, TempMax, TempMin, MaxWind, AveWind, AveHumidity, RainDay );
	}

   void CForecastHistorizer::ClearAllPeriods( void )
   {
      m_content->ClearAllPeriods();
   }

	std::string CForecastHistorizer::formatValue() const
	{
	   return !m_content ? std::string() : m_content->formatValue();
	}

	const EMeasureType& CForecastHistorizer::getMeasureType() const
	{
	   static const EMeasureType MeasureType(EMeasureType::kAbsolute);
	   return MeasureType;
	}	

   CDataContainer CForecastHistorizer::getTypeInfo() const
   {
      return CDataContainer();
   }
	
} } } } // namespace shared::plugin::yPluginApi::historization

