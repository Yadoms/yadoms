#include "stdafx.h"
#include "RunningPeriod.h"
#include "KeywordException.hpp"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CRunningPeriod::CRunningPeriod(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName)
   : m_runningPeriod(boost::make_shared<linky::specificHistorizers::CPeriod>(KeyWordName)),
     m_isDeveloperMode(api->getYadomsInformation()->developperMode())
{
}

void CRunningPeriod::set(const linky::specificHistorizers::EPeriod value) const
{
   try
   {
      //auto it = EEnumPeriod.find(value);
      //if (it != EEnumPeriod.end())
      //{
         m_runningPeriod->set(value);

         if (m_isDeveloperMode) YADOMS_LOG(information) << m_runningPeriod->getKeyword() << "=" << m_runningPeriod->get() ;
      //}
      //else
      //   throw CKeywordException("Keyword " + m_runningPeriod->getKeyword() + " could not be set");
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

