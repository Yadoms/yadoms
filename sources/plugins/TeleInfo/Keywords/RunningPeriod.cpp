#include "stdafx.h"
#include "RunningPeriod.h"
#include "KeywordException.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRunningPeriod::CRunningPeriod(boost::shared_ptr<yApi::IYPluginApi> api,
                               std::string PluginName,
                               std::string KeyWordName,
                               shared::CDataContainer details)
   : m_PluginName(PluginName),
     m_runningPeriod(boost::make_shared<teleInfo::specificHistorizers::CPeriod>(KeyWordName))
{
   if (!api->keywordExists(m_PluginName, m_runningPeriod))
      api->declareKeyword(m_PluginName, m_runningPeriod, details);
}

void CRunningPeriod::SetValue(std::string& Value) const
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
      EnumPeriod::const_iterator it = EEnumPeriod.find(Value);
      if (it != EEnumPeriod.end())
      {
         m_runningPeriod->set(static_cast<teleInfo::specificHistorizers::EPeriod>(it->second));

         std::cout << m_runningPeriod->getKeyword() << "=" << m_runningPeriod->get() << std::endl;
      }
      else
         throw CKeywordException("Keyword " + m_runningPeriod->getKeyword() + " could not be set");
   }
   catch (shared::exception::CException& e)
   {
      std::cout << e.what() << std::endl << std::endl;
   }
}

CRunningPeriod::~CRunningPeriod()
{
}

boost::shared_ptr<yApi::historization::IHistorizable> CRunningPeriod::GetHistorizable() const
{
   return m_runningPeriod;
}

