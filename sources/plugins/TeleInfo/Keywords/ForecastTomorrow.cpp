#include "stdafx.h"
#include "ForecastTomorrow.h"
#include "KeywordException.hpp"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecastTomorrow::CForecastTomorrow(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName)
   : m_forecastPeriod(boost::make_shared<teleInfo::specificHistorizers::CColor>(KeyWordName)),
   m_isChanged(true)
{
}

void CForecastTomorrow::set(const std::string& Value)
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
         auto temp = static_cast<teleInfo::specificHistorizers::EColor>(it->second);

         if (m_forecastPeriod->get() != temp)
         {
            m_isChanged = true;
            m_forecastPeriod->set(temp);
         }
         else
            m_isChanged = false;
      }
      else
         throw CKeywordException("Keyword " + m_forecastPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(information) << e.what();
   }
}

void CForecastTomorrow::set(const teleInfo::specificHistorizers::EColor& Value)
{
   if (m_forecastPeriod->get() != Value)
   {
      m_isChanged = true;
      m_forecastPeriod->set(Value);
   }
   else
      m_isChanged = false;
}

CForecastTomorrow::~CForecastTomorrow()
{
}

boost::shared_ptr<yApi::historization::IHistorizable> CForecastTomorrow::GetHistorizable() const
{
   return m_forecastPeriod;
}

bool CForecastTomorrow::isChanged() const {
   return m_isChanged;
}