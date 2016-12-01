#include "stdafx.h"
#include "ForecastTomorrow.h"
#include "KeywordException.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecastTomorrow::CForecastTomorrow(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName)
   : m_forecastPeriod(boost::make_shared<teleInfo::specificHistorizers::CColor>(KeyWordName)),
     m_isDeveloperMode(api->getYadomsInformation()->developperMode())
{
}

void CForecastTomorrow::set(const std::string& Value) const
{
   static const EnumColorMap EEnumColorMap = boost::assign::map_list_of
      ("----", teleInfo::specificHistorizers::EColor::kNOTDEFINED)
      ("BLEU", teleInfo::specificHistorizers::EColor::kBLUE)
      ("BLAN", teleInfo::specificHistorizers::EColor::kWHITE)
      ("ROUG", teleInfo::specificHistorizers::EColor::kRED);

   try
   {
      auto it = EEnumColorMap.find(Value);
      if (it != EEnumColorMap.end())
      {
         m_forecastPeriod->set(static_cast<teleInfo::specificHistorizers::EColor>(it->second));

         if (m_isDeveloperMode) std::cout << m_forecastPeriod->getKeyword() << "=" << m_forecastPeriod->get() << std::endl;
      }
      else
         throw CKeywordException("Keyword " + m_forecastPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl << std::endl;
   }
}

CForecastTomorrow::~CForecastTomorrow()
{
}

boost::shared_ptr<yApi::historization::IHistorizable> CForecastTomorrow::GetHistorizable() const
{
   return m_forecastPeriod;
}

