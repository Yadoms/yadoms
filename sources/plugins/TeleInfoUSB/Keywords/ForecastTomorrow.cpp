#include "stdafx.h"
#include "ForecastTomorrow.h"
#include "KeywordException.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecastTomorrow::CForecastTomorrow(boost::shared_ptr<yApi::IYPluginApi> api,
                                     std::string PluginName,
                                     std::string KeyWordName,
                                     shared::CDataContainer details)
   : m_PluginName(PluginName),
     m_forecastPeriod(new teleInfoUSB::specificHistorizers::CColor(KeyWordName))
{
   if (!api->keywordExists(m_PluginName, m_forecastPeriod->getKeyword()))
      api->declareKeyword(m_PluginName, *m_forecastPeriod, details);
}

void CForecastTomorrow::SetValue(std::string& Value) const
{
   static const EnumColorMap EEnumColorMap = boost::assign::map_list_of
      ("----", teleInfoUSB::specificHistorizers::EColor::kNOTDEFINED)
      ("BLEU", teleInfoUSB::specificHistorizers::EColor::kBLUE)
      ("BLAN", teleInfoUSB::specificHistorizers::EColor::kWHITE)
      ("ROUG", teleInfoUSB::specificHistorizers::EColor::kRED);

   try
   {
      auto it = EEnumColorMap.find(Value);
      if (it != EEnumColorMap.end())
      {
         m_forecastPeriod->set(static_cast<teleInfoUSB::specificHistorizers::EColor>(it->second));

         std::cout << m_forecastPeriod->getKeyword() << "=" << m_forecastPeriod->get() << std::endl;
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

