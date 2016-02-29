#include "stdafx.h"
#include "ConditionFormatter.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/DataContainer.h>
#include <shared/Log.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{

CConditionFormatter::CConditionFormatter( void )
{}

void CConditionFormatter::AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
   )
{
	try
	{
		m_Units.get ( UnitName );
	}
	catch ( shared::exception::CException& )
	{  // If Exception, we create the unit
		m_Units.set ( UnitName, UnitValue );
	}
}

void CConditionFormatter::SetPeriod(
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
   m_ConditionFrame.set ("Time", Time);
   m_ConditionFrame.set ("WeatherCondition", WeatherCondition);
   m_ConditionFrame.set ("Temp", Temp);
   m_ConditionFrame.set ("Pressure", Pressure);
   m_ConditionFrame.set ("Visibility", Visibility);
   m_ConditionFrame.set ("UV", UV);
   m_ConditionFrame.set ("DewPoint", DewPoint);
   m_ConditionFrame.set ("MaxWind", MaxWind);
   m_ConditionFrame.set ("AveWind", AveWind);
   m_ConditionFrame.set ("AveWindDegrees", AveWindDegrees);
   m_ConditionFrame.set ("Humidity", Humidity);
   m_ConditionFrame.set ("RainDay", RainDay);
   m_ConditionFrame.set ("FeelsLike", Feelslike);
   m_ConditionFrame.set ("WindChill", WindChill);
}

void CConditionFormatter::SetCityName ( const std::string & CityName )
{
   m_Localisation = CityName;
}

CConditionFormatter::~CConditionFormatter()
{}

std::string CConditionFormatter::formatValue() const
{
   CDataContainer Temp;

   Temp.set ("Units", m_Units);
   Temp.set ("city", m_Localisation);
   Temp.set ("Conditions", m_ConditionFrame );

   return Temp.serialize();
}

} } } } // namespace shared::plugin::yPluginApi::historization

