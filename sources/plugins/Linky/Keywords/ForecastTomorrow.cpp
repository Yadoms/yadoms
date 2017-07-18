#include "stdafx.h"
#include "ForecastTomorrow.h"
#include "KeywordException.hpp"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CForecastTomorrow::CForecastTomorrow(boost::shared_ptr<yApi::IYPluginApi> api, const std::string& KeyWordName)
   : m_forecastPeriod(boost::make_shared<linky::specificHistorizers::CColor>(KeyWordName)),
     m_isDeveloperMode(api->getYadomsInformation()->developperMode())
{
}

void CForecastTomorrow::set(const unsigned char value) const
{
   try
   {
      linky::specificHistorizers::EColor forecast(value);
      m_forecastPeriod->set(forecast);
      if (m_isDeveloperMode) YADOMS_LOG(information) << m_forecastPeriod->getKeyword() << "=" << m_forecastPeriod->get();
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

