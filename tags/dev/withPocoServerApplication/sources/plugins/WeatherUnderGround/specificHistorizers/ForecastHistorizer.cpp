#include "stdafx.h"
#include "ForecastHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   const CStandardCapacity& ForecastCapacity = CStandardCapacity("Forecast", CStandardUnits::NoUnits, EKeywordDataType::kNoData);

   
   CForecastHistorizer::CForecastHistorizer(const std::string& keywordName, const EKeywordAccessMode& accessMode)
   :m_keywordName(keywordName), m_accessMode(accessMode)
   {
	   m_content.reset(new CForecastFormatter());
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

	void CForecastHistorizer::AddPeriod(
		          const std::string& WeatherCondition, 
	             const std::string& TempMax, 
					 const std::string& TempMin,
					 const std::string& MaxWind,
					 const std::string& AveWind,
					 const std::string& AveHumidity,
                const std::string& RainDay
					 )
	{
		m_content->AddPeriod( WeatherCondition, TempMax, TempMin, MaxWind, AveWind, AveHumidity, RainDay );
	}

	std::string CForecastHistorizer::formatValue() const
	{
	   m_content->Finalize();
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

