#include "stdafx.h"
#include "RunningPeriod.h"
#include "KeywordException.hpp"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRunningPeriod::CRunningPeriod(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName)
   : m_runningPeriod(boost::make_shared<linky::specificHistorizer::CPeriod>(KeyWordName)),
   m_isChanged(true)
{
}

void CRunningPeriod::set(const std::string& value)
{
   static const EnumPeriod EEnumPeriod = boost::assign::map_list_of
      ("TH..", linky::specificHistorizer::EPeriod::kAllHours)
      ("HC..", linky::specificHistorizer::EPeriod::kLowCostHours)
      ("HP..", linky::specificHistorizer::EPeriod::kPeakCostHours)
      ("HN..", linky::specificHistorizer::EPeriod::kNormalCostHours)
      ("PM..", linky::specificHistorizer::EPeriod::kMobilePeakCostHours)
      ("HCJB", linky::specificHistorizer::EPeriod::kLowCostBlueDays)
      ("HCJW", linky::specificHistorizer::EPeriod::kLowCostWhiteDays)
      ("HCJR", linky::specificHistorizer::EPeriod::kLowCostRedDays)
      ("HPJB", linky::specificHistorizer::EPeriod::kNormalCostBlueDays)
      ("HPJW", linky::specificHistorizer::EPeriod::kNormalCostWhiteDays)
      ("HPJR", linky::specificHistorizer::EPeriod::kNormalCostRedDays);

   try
   {
      auto it = EEnumPeriod.find(value);
      auto temp = static_cast<linky::specificHistorizer::EPeriod>(it->second);
      if (it != EEnumPeriod.end())
      {
         if (m_runningPeriod->get() != temp) {
            m_runningPeriod->set(temp);
            m_isChanged = true;
         }
         else
            m_isChanged = false;
      }
      else
         throw CKeywordException("Keyword " + m_runningPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      YADOMS_LOG(information) << e.what();
   }
}

bool CRunningPeriod::isChanged() const {
   return m_isChanged;
}

CRunningPeriod::~CRunningPeriod()
{
}

boost::shared_ptr<yApi::historization::IHistorizable> CRunningPeriod::GetHistorizable() const
{
   return m_runningPeriod;
}

