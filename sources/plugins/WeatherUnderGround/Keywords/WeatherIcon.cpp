#include "stdafx.h"
#include "WeatherIcon.h"
#include "KeywordException.hpp"
#include "../ForecastDays.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CWeatherIcon::CWeatherIcon(std::string PluginName,
                           std::string KeyWordName)
   : m_PluginName(PluginName),
     m_weathercondition(boost::make_shared<yApi::historization::CWeatherCondition>(KeyWordName))
{
}

void CWeatherIcon::Initialize(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details) const
{
   if (!api->keywordExists(m_PluginName, m_weathercondition))
      api->declareKeyword(m_PluginName, m_weathercondition, details);
}

CWeatherIcon::~CWeatherIcon()
{
}

void CWeatherIcon::SetValue(const shared::CDataContainer& ValueContainer,
                            const std::string& filter) const
{
   try
   {
      auto it = weatherunderground::helper::EEnumTypeNames.find(ValueContainer.get<std::string>(filter));
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

boost::shared_ptr<yApi::historization::IHistorizable> CWeatherIcon::GetHistorizable() const
{
   return m_weathercondition;
}

