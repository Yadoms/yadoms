#include "stdafx.h"
#include "WeatherIcon.h"
#include "KeywordException.hpp"
#include "../ForecastDays.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWeatherIcon::CWeatherIcon(std::string pluginName,
                           std::string keyWordName)
   : m_pluginName(pluginName),
     m_weathercondition(boost::make_shared<yApi::historization::CWeatherCondition>(keyWordName))
{
}

void CWeatherIcon::initialize(boost::shared_ptr<yApi::IYPluginApi> api,
                              shared::CDataContainer details) const
{
   if (!api->keywordExists(m_pluginName, m_weathercondition))
      api->declareKeyword(m_pluginName, m_weathercondition, details);
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

