#include "stdafx.h"
#include "WeatherIcon.h"
#include "specificHistorizers/ForecastHelpers.h"
#include "KeywordException.hpp"

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

         std::cout << m_weathercondition->getKeyword() << "=" << m_weathercondition->get() << std::endl;
      }
      else
         throw CKeywordException("Keyword WeatherIcon could not be set");
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl << std::endl;
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CWeatherIcon::getHistorizable() const
{
   return m_weathercondition;
}

