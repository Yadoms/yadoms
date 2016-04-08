#include "stdafx.h"
#include "ConditionHistorizer.h"
#include <shared/plugin/yPluginApi/StandardUnits.h>
   
   const yApi::CStandardCapacity& ConditionCapacity = yApi::CStandardCapacity("Condition", yApi::CStandardUnits::NoUnits, yApi::EKeywordDataType::kNoData);

   CConditionHistorizer::CConditionHistorizer(const std::string& keywordName, 
                                            const yApi::EKeywordAccessMode& accessMode)
   :m_keywordName(keywordName), m_accessMode(accessMode)
   {
	   m_content.reset(new CConditionFormatter( ));
   }

   CConditionHistorizer::~CConditionHistorizer()
   {}

	const std::string& CConditionHistorizer::getKeyword() const
	{
	   return m_keywordName;
	}

	const yApi::CStandardCapacity& CConditionHistorizer::getCapacity() const
	{
	   return ConditionCapacity;
	}
	
	const yApi::EKeywordAccessMode& CConditionHistorizer::getAccessMode() const
	{
	   return m_accessMode;
	}

void CConditionHistorizer::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
   m_content->AddUnit ( UnitName, UnitValue );
}

	void CConditionHistorizer::SetPeriod(
			const std::string& Time,
			const std::string& WeatherCondition, 
			const std::string& Temp, 
			const std::string& Pressure,
			const std::string& Visibility,
			const std::string& UV,
			const std::string& DewPoint,
			const std::string& MaxWind,
			const std::string& AveWind,
			const std::string& AveWindDegrees,
			const std::string& Humidity,
			const std::string& RainDay,
			const std::string& Feelslike,
			const std::string& WindChill
					 )
	{
		m_content->SetPeriod( Time, WeatherCondition, Temp, Pressure, Visibility, UV, DewPoint, MaxWind, AveWind,AveWindDegrees, Humidity, RainDay, Feelslike, WindChill );
	}

	std::string CConditionHistorizer::formatValue() const
	{
	   return !m_content ? std::string() : m_content->formatValue();
	}

	const yApi::historization::EMeasureType& CConditionHistorizer::getMeasureType() const
	{
	   static const yApi::historization::EMeasureType MeasureType(yApi::historization::EMeasureType::kAbsolute);
	   return MeasureType;
	}	

   shared::CDataContainer CConditionHistorizer::getTypeInfo() const
   {
      return shared::CDataContainer();
   }
	
void CConditionHistorizer::SetCityName ( const std::string& CityName )
{
   m_content->SetCityName ( CityName );
}