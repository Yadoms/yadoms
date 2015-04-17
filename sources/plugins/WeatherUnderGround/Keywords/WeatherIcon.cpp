#include "stdafx.h"
#include "WeatherIcon.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWeatherIcon::CWeatherIcon( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_weathercondition( new yApi::historization::CWeatherCondition(KeyWordName) )
{}

void CWeatherIcon::Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const
{
   if (!context->keywordExists( m_PluginName, m_weathercondition->getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CWeatherIcon::~CWeatherIcon()
{
}

void CWeatherIcon::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
   static const EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("cloudy", yApi::historization::EWeatherCondition::kCloudy)
	  ("mostlycloudy", yApi::historization::EWeatherCondition::kCloudy)
	  ("partlycloudy", yApi::historization::EWeatherCondition::kCloudy)
	  ("mostlysunny", yApi::historization::EWeatherCondition::kCloudy)
      ("snow", yApi::historization::EWeatherCondition::kSnow)
	  ("flurries", yApi::historization::EWeatherCondition::kSnow)
	  ("chanceflurries", yApi::historization::EWeatherCondition::kSnow)
	  ("chancesnow", yApi::historization::EWeatherCondition::kSnow)
	  ("sunny", yApi::historization::EWeatherCondition::kSunny)
	  ("clear", yApi::historization::EWeatherCondition::kSunny)
	  ("rain", yApi::historization::EWeatherCondition::kRain)
	  ("chancerain", yApi::historization::EWeatherCondition::kRain)
	  ("sleet", yApi::historization::EWeatherCondition::kSleet)
	  ("chancesleet", yApi::historization::EWeatherCondition::kSleet)
	  ("tstorms", yApi::historization::EWeatherCondition::kStorm)
	  ("chancestorm", yApi::historization::EWeatherCondition::kStorm)
	  ("fog", yApi::historization::EWeatherCondition::kFog)
	  ("hazy", yApi::historization::EWeatherCondition::kFog)
	  ("hazy", yApi::historization::EWeatherCondition::kFog)
      ("partlysunny", yApi::historization::EWeatherCondition::kPartlySunny)
	  ("nt_chancerain", yApi::historization::EWeatherCondition::kNight_Rain)
	  ("nt_clear", yApi::historization::EWeatherCondition::kNight_Clear)
	  ("nt_cloudy", yApi::historization::EWeatherCondition::kNight_Cloudy)
	  ("nt_mostlycloudy", yApi::historization::EWeatherCondition::kNight_Cloudy)
	  ("nt_partlycloudy", yApi::historization::EWeatherCondition::kNight_Cloudy)
	  ("nt_snow", yApi::historization::EWeatherCondition::kNight_Snow);

      EnumValuesNames::const_iterator it = EEnumTypeNames.find( ValueContainer.get<std::string>( filter ) );
      if (it != EEnumTypeNames.end())
	  {
         m_weathercondition->set( (yApi::historization::EWeatherCondition)(it->second) );

		 YADOMS_LOG(debug) << m_weathercondition->getKeyword() << "=" << m_weathercondition->get();
	  }
	  else
		  throw; //TODO : Emettre une vrai Exception
}

void CWeatherIcon::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, *m_weathercondition);
}

boost::shared_ptr<yApi::historization::IHistorizable> CWeatherIcon::GetHistorizable() const
{
	return m_weathercondition;
}