#include "stdafx.h"
#include "OledScreenControl.h"
#include <shared/Log.h>
#include <shared/http/HttpMethods.h>

COledScreenControl::COledScreenControl()
	:m_on_event(boost::make_shared<yApi::historization::CEvent>("switch_on", yApi::EKeywordAccessMode::kGetSet)),
	 m_off_event(boost::make_shared<yApi::historization::CEvent>("switch_off", yApi::EKeywordAccessMode::kGetSet)),
	 m_clear_event(boost::make_shared<yApi::historization::CEvent>("clear", yApi::EKeywordAccessMode::kGetSet))
{
	
}

COledScreenControl::~COledScreenControl()
{
	
}
boost::shared_ptr< yApi::historization::CEvent> COledScreenControl::get_on_event() const
{
	return m_on_event;
}

boost::shared_ptr< yApi::historization::CEvent> COledScreenControl::get_off_event() const
{
	return m_off_event;

}
boost::shared_ptr< yApi::historization::CEvent> COledScreenControl::get_clear_event() const
{
	return m_clear_event;
}
