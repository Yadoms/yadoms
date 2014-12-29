#include "stdafx.h"
#include "WUConfiguration.h"
#include <shared/StringExtension.h>
#include <shared/Log.h>

CWUConfiguration::~CWUConfiguration()
{
   m_isTemperatureEnabled = false;
   m_isDewPointEnabled = false;
   m_isPressureEnabled = false;
   m_isHumidityEnabled = false;
   m_isVisibilityEnabled = false;
   m_isUVEnabled = false;
   m_isRain1hrEnabled = false;
   m_isRainDayEnabled = false;
   m_isWindEnabled = false;
   m_isFeelsLikeEnabled = false;
   m_isWinChillEnabled = false;
   m_isWeatherConditionIconEnabled = false;
}

void CWUConfiguration::initializeWith(const shared::CDataContainer &data)
{
   m_data.initializeWith(data);

   try
   {
	   m_isTemperatureEnabled = m_data.get<bool>("Information.content.Temperature");
	   m_isDewPointEnabled    = m_data.get<bool>("Information.content.DewPoint");
	   m_isPressureEnabled    = m_data.get<bool>("Information.content.Pressure");
	   m_isHumidityEnabled    = m_data.get<bool>("Information.content.Humidity");
	   m_isVisibilityEnabled  = m_data.get<bool>("Information.content.Visibility");
	   m_isUVEnabled          = m_data.get<bool>("Information.content.UV");
	   m_isRain1hrEnabled     = m_data.get<bool>("Information.content.Rain1h");
	   m_isRainDayEnabled     = m_data.get<bool>("Information.content.RainDay");
	   m_isWindEnabled        = m_data.get<bool>("Information.content.WindInformation.content.Wind");
	   m_isWeatherConditionIconEnabled = m_data.get<bool>("Information.content.WeatherConditionIcon");
	   m_isFeelsLikeEnabled   = m_data.get<bool>("Information.content.WindInformation.content.Feelslike");
	   m_isWinChillEnabled    = m_data.get<bool>("Information.content.WindInformation.content.WindChill");
   }
	catch (boost::thread_interrupted&)
	{
		YADOMS_LOG(error) << "ERROR : Plugin Configuration could not be loaded"  << std::endl;
	}
}

std::string CWUConfiguration::getAPIKey() const
{
   return m_data.get<std::string>("APIKey");
}

std::string CWUConfiguration::getLocalisation() const
{
   return m_data.get<std::string>("Localisation");
}

bool CWUConfiguration::IsTemperatureEnabled (void) const {return m_isTemperatureEnabled;}
bool CWUConfiguration::IsDewPointEnabled (void) const    {return m_isDewPointEnabled;}
bool CWUConfiguration::IsPressureEnabled (void) const    {return m_isPressureEnabled;}
bool CWUConfiguration::IsHumidityEnabled (void) const    {return m_isHumidityEnabled;}
bool CWUConfiguration::IsVisibilityEnabled (void) const  {return m_isVisibilityEnabled;}
bool CWUConfiguration::IsUVEnabled (void) const          {return m_isUVEnabled;}
bool CWUConfiguration::IsRain1hrEnabled (void) const     {return m_isRain1hrEnabled;}
bool CWUConfiguration::IsRainDayEnabled (void) const     {return m_isRainDayEnabled;}
bool CWUConfiguration::IsWindEnabled (void) const        {return m_isWindEnabled;}
bool CWUConfiguration::IsFeelsLikeEnabled (void) const   {return m_isFeelsLikeEnabled;}
bool CWUConfiguration::IsWinChillEnabled (void) const    {return m_isWinChillEnabled;}
bool CWUConfiguration::IsWeatherConditionIconEnabled (void ) const {return m_isWeatherConditionIconEnabled;}