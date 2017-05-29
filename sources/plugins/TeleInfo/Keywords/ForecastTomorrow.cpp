#include "stdafx.h"
#include "ForecastTomorrow.h"
#include "KeywordException.hpp"
#include <shared/Log.h>

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
      ("----", teleInfo::specificHistorizers::ETeleInfoColor::kNOTDEFINED)
      ("BLEU", teleInfo::specificHistorizers::ETeleInfoColor::kBLUE)
      ("BLAN", teleInfo::specificHistorizers::ETeleInfoColor::kWHITE)
      ("ROUG", teleInfo::specificHistorizers::ETeleInfoColor::kRED);

   try
   {
      auto it = EEnumColorMap.find(Value);
      if (it != EEnumColorMap.end())
      {
         m_forecastPeriod->set(static_cast<teleInfo::specificHistorizers::ETeleInfoColor>(it->second));

         if (m_isDeveloperMode) YADOMS_LOG(information) << m_forecastPeriod->getKeyword() << "=" << m_forecastPeriod->get() ;
      }
      else
         throw CKeywordException("Keyword " + m_forecastPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(information) << e.what();
   }
}

CForecastTomorrow::~CForecastTomorrow()
{
}

boost::shared_ptr<yApi::historization::IHistorizable> CForecastTomorrow::GetHistorizable() const
{
   return m_forecastPeriod;
}

