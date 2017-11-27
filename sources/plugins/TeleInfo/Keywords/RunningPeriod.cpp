#include "stdafx.h"
#include "RunningPeriod.h"
#include "KeywordException.hpp"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRunningPeriod::CRunningPeriod(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName)
   : m_runningPeriod(boost::make_shared<teleInfo::specificHistorizers::CPeriod>(KeyWordName))
{
}

void CRunningPeriod::set(const std::string& value) const
{
   static const EnumPeriod EEnumPeriod = boost::assign::map_list_of
      ("TH..", teleInfo::specificHistorizers::EPeriod::kAllHours)
      ("HC..", teleInfo::specificHistorizers::EPeriod::kLowCostHours)
      ("HP..", teleInfo::specificHistorizers::EPeriod::kPeakCostHours)
      ("HN..", teleInfo::specificHistorizers::EPeriod::kNormalCostHours)
      ("PM..", teleInfo::specificHistorizers::EPeriod::kMobilePeakCostHours)
      ("HCJB", teleInfo::specificHistorizers::EPeriod::kLowCostBlueDays)
      ("HCJW", teleInfo::specificHistorizers::EPeriod::kLowCostWhiteDays)
      ("HCJR", teleInfo::specificHistorizers::EPeriod::kLowCostRedDays)
      ("HPJB", teleInfo::specificHistorizers::EPeriod::kNormalCostBlueDays)
      ("HPJW", teleInfo::specificHistorizers::EPeriod::kNormalCostWhiteDays)
      ("HPJR", teleInfo::specificHistorizers::EPeriod::kNormalCostRedDays);

   try
   {
      auto it = EEnumPeriod.find(value);
      if (it != EEnumPeriod.end())
      {
         m_runningPeriod->set(static_cast<teleInfo::specificHistorizers::EPeriod>(it->second));
         YADOMS_LOG(trace) << m_runningPeriod->getKeyword() << "=" << m_runningPeriod->get() ;
      }
      else
         throw CKeywordException("Keyword " + m_runningPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(information) << e.what();
   }
}

CRunningPeriod::~CRunningPeriod()
{
}

boost::shared_ptr<yApi::historization::IHistorizable> CRunningPeriod::GetHistorizable() const
{
   return m_runningPeriod;
}

