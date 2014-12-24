#include "stdafx.h"
#include "Humidity.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace WUCapacities
{

CHumidity::CHumidity(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName)
   :m_humidity("Humidity")
{
	m_PluginName = PluginName;

	if (!context->keywordExists( m_PluginName, m_humidity.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CHumidity::~CHumidity()
{
}

void CHumidity::GetValue(double temp)
{
   m_humidity.set( (int) temp);
}

void CHumidity::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_humidity);
}

void CHumidity::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_humidity);
}
} // namespace WUCapacities