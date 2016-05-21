#include "stdafx.h"
#include "ForecastTomorrow.h"
#include "KeywordException.hpp"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecastTomorrow::CForecastTomorrow( boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName, shared::CDataContainer details )
   :m_PluginName ( PluginName ), m_forecastPeriod( new teleInfoUSB::specificHistorizers::CColor(KeyWordName) )
{
   if (!context->keywordExists( m_PluginName, m_forecastPeriod->getKeyword()))
      context->declareKeyword ( m_PluginName, *m_forecastPeriod, details );
}

void CForecastTomorrow::SetValue( std::string& Value )
{
   static const EnumColorMap EEnumColorMap = boost::assign::map_list_of
      ("----", teleInfoUSB::specificHistorizers::EColor::kNOTDEFINED)
      ("BLEU", teleInfoUSB::specificHistorizers::EColor::kBLUE)
      ("BLAN", teleInfoUSB::specificHistorizers::EColor::kWHITE)
      ("ROUG", teleInfoUSB::specificHistorizers::EColor::kRED)
      ;

   try
   {
      EnumColorMap::const_iterator it = EEnumColorMap.find(Value);
      if (it != EEnumColorMap.end())
      {
         m_forecastPeriod->set((teleInfoUSB::specificHistorizers::EColor)(it->second));

         YADOMS_LOG(debug) << m_forecastPeriod->getKeyword() << "=" << m_forecastPeriod->get();
      }
      else
         throw CKeywordException("Keyword " + m_forecastPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(warning) << e.what() << std::endl;
   }
}

CForecastTomorrow::~CForecastTomorrow()
{}

boost::shared_ptr<yApi::historization::IHistorizable> CForecastTomorrow::GetHistorizable() const
{
	return m_forecastPeriod;
}
