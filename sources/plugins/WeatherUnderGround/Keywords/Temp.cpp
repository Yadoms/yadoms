#include "stdafx.h"
#include "Temp.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CTemp::CTemp( std::string PluginName ,std::string KeyWordName )
   :m_PluginName ( PluginName ), m_temperature( new yApi::historization::CTemperature(KeyWordName, yApi::EKeywordAccessMode::kGet) )
{}

void CTemp::Initialize( boost::shared_ptr<yApi::IYPluginApi> context, shared::CDataContainer details ) const
{
   if (!context->keywordExists( m_PluginName, m_temperature->getKeyword()))
      context->declareKeyword( m_PluginName, *m_temperature, details );
}

CTemp::~CTemp()
{}

void CTemp::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
   if (ValueContainer.get<std::string>( filter ) == "NA")
      YADOMS_LOG(information) << m_temperature->getKeyword() << " : NA => Value not registered";
   else
   {
	   m_temperature->set(ValueContainer.get<double>( filter ));
	   YADOMS_LOG(debug) << m_temperature->getKeyword() << "=" << m_temperature->get() << " celcius degrees";
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CTemp::GetHistorizable() const
{
	return m_temperature;
}
