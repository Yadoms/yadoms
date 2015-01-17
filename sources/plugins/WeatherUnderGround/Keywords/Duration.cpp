#include "stdafx.h"
#include "Duration.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include <boost/date_time.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

CDuration::CDuration(boost::shared_ptr<yApi::IYPluginApi> context, std::string PluginName, std::string KeyWordName)
   :m_PluginName ( PluginName ), m_duration( KeyWordName )
{
   	if (!context->keywordExists( m_PluginName, m_duration.getKeyword()))
	{
      DeclareKeywords ( context );
	}
}

CDuration::~CDuration()
{}

void CDuration::DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const
{
	context->declareKeyword(m_PluginName, m_duration);
}

void CDuration::SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter)
{
	m_duration.set(ValueContainer.get<boost::posix_time::time_duration>( filter ) * 24 );

	YADOMS_LOG(debug) << m_duration.getKeyword() << "=" << boost::posix_time::to_simple_string(m_duration.get());
}

void CDuration::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_PluginName, m_duration);
}