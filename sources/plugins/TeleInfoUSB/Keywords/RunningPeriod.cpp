#include "stdafx.h"
#include "RunningPeriod.h"
#include "KeywordException.hpp"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRunningPeriod::CRunningPeriod( boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_runningPeriod( new teleInfoUSB::specificHistorizers::CPeriod(KeyWordName) )
{
   if (!context->keywordExists( m_PluginName, m_runningPeriod->getKeyword()))
      context->declareKeyword ( m_PluginName, *m_runningPeriod );
}

void CRunningPeriod::SetValue( std::string& Value )
{
   static const EnumPeriod EEnumPeriod = boost::assign::map_list_of
      ("TH..", teleInfoUSB::specificHistorizers::EPeriod::kAllHours)
      ("HC..", teleInfoUSB::specificHistorizers::EPeriod::kLowCostHours)
      ("HP..", teleInfoUSB::specificHistorizers::EPeriod::kPeakCostHours)
      ("HN..", teleInfoUSB::specificHistorizers::EPeriod::kNormalCostHours)
      ("PM..", teleInfoUSB::specificHistorizers::EPeriod::kMobilePeakCostHours)
      ("HCJB", teleInfoUSB::specificHistorizers::EPeriod::kLowCostBlueDays)
      ("HCJW", teleInfoUSB::specificHistorizers::EPeriod::kLowCostWhiteDays)
      ("HCJR", teleInfoUSB::specificHistorizers::EPeriod::kLowCostRedDays)
      ("HPJB", teleInfoUSB::specificHistorizers::EPeriod::kNormalCostBlueDays)
      ("HPJW", teleInfoUSB::specificHistorizers::EPeriod::kNormalCostWhiteDays)
      ("HPJR", teleInfoUSB::specificHistorizers::EPeriod::kNormalCostRedDays)
      ;

   try
   {
      EnumPeriod::const_iterator it = EEnumPeriod.find(Value);
      if (it != EEnumPeriod.end())
      {
         m_runningPeriod->set(static_cast<teleInfoUSB::specificHistorizers::EPeriod>(it->second));

         YADOMS_LOG(debug) << m_runningPeriod->getKeyword() << "=" << m_runningPeriod->get();
      }
      else
         throw CKeywordException("Keyword " + m_runningPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(warning) << e.what() << std::endl;
   }
}

CRunningPeriod::~CRunningPeriod()
{}

boost::shared_ptr<yApi::historization::IHistorizable> CRunningPeriod::GetHistorizable() const
{
	return m_runningPeriod;
}