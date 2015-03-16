#include "stdafx.h"
#include "ForecastHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   const shared::plugin::yPluginApi::CStandardCapacity& ForecastCapacity = shared::plugin::yPluginApi::CStandardCapacity("Forecast", shared::plugin::yPluginApi::CStandardUnits::NoUnits, shared::plugin::yPluginApi::EKeywordDataType::kNoData);

   
   CForecastHistorizer::CForecastHistorizer(const std::string& keywordName, const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode)
   :m_keywordName(keywordName), m_accessMode(accessMode)
   {
	   m_content.reset(new shared::plugin::yPluginApi::historization::CForecastFormatter());
   }

   CForecastHistorizer::~CForecastHistorizer()
   {}

	const std::string& CForecastHistorizer::getKeyword() const
	{
	   return m_keywordName;
	}

	const shared::plugin::yPluginApi::CStandardCapacity& CForecastHistorizer::getCapacity() const
	{
	   return ForecastCapacity;
	}
	
	const shared::plugin::yPluginApi::EKeywordAccessMode& CForecastHistorizer::getAccessMode() const
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

	const shared::plugin::yPluginApi::historization::EMeasureType& CForecastHistorizer::getMeasureType() const
	{
	   static const shared::plugin::yPluginApi::historization::EMeasureType MeasureType(shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute);
	   return MeasureType;
	}	
	
} } } } // namespace shared::plugin::yPluginApi::historization

