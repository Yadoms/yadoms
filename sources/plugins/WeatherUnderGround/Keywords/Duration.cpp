#include "stdafx.h"
#include "Duration.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <boost/date_time.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CDuration::CDuration( std::string PluginName, std::string KeyWordName )
   :m_PluginName ( PluginName ), m_duration( new yApi::historization::CDuration(KeyWordName) )
{}


void CDuration::Initialize( boost::shared_ptr<yApi::IYPluginApi> context, shared::CDataContainer details ) const
{
   if (!context->keywordExists( m_PluginName, m_duration->getKeyword()))
	{
      context->declareKeyword( m_PluginName, *m_duration, details );
	}
}

CDuration::~CDuration()
{}

void CDuration::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_duration->set(ValueContainer.get<boost::posix_time::time_duration>( filter ) * 24 );

	YADOMS_LOG(debug) << m_duration->getKeyword() << "=" << boost::posix_time::to_simple_string(m_duration->get());
}

boost::shared_ptr<yApi::historization::IHistorizable> CDuration::GetHistorizable() const
{
	return m_duration;
}