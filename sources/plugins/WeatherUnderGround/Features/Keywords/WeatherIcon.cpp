#include "stdafx.h"
#include "WeatherIcon.h"
#include "specificHistorizers/ForecastHelpers.h"
#include "KeywordException.hpp"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWeatherIcon::CWeatherIcon(const std::string& pluginName,
                           const std::string& keyWordName)
   : m_pluginName(pluginName),
     m_weathercondition(boost::make_shared<yApi::historization::CWeatherCondition>(keyWordName))
{
}

CWeatherIcon::~CWeatherIcon()
{
}

void CWeatherIcon::setValue(const shared::CDataContainer& valueContainer,
                            const std::string& filter) const
{
   try
   {
      auto it = weatherunderground::helper::EEnumTypeNames.find(valueContainer.get<std::string>(filter));
      if (it != weatherunderground::helper::EEnumTypeNames.end())
      {
         m_weathercondition->set(static_cast<yApi::historization::EWeatherCondition>(it->second));

         YADOMS_LOG(information) << m_weathercondition->getKeyword() << "=" << m_weathercondition->get() ;
      }
      else
         throw CKeywordException("Keyword WeatherIcon could not be set");
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(information) << e.what() ;
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CWeatherIcon::getHistorizable() const
{
   return m_weathercondition;
}

