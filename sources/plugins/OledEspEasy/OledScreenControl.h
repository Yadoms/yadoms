#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "OledScreenLine.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


//--------------------------------------------------------------
/// \brief	Configuration of the empty plugin
//--------------------------------------------------------------
class COledScreenControl
{
public:
	COledScreenControl();
	virtual ~COledScreenControl();

	boost::shared_ptr< yApi::historization::CEvent> get_on_event() const;
	boost::shared_ptr< yApi::historization::CEvent> get_off_event() const;
	boost::shared_ptr< yApi::historization::CEvent> get_clear_event() const;
private:
	boost::shared_ptr< yApi::historization::CEvent> m_on_event;
	boost::shared_ptr< yApi::historization::CEvent> m_off_event;
	boost::shared_ptr< yApi::historization::CEvent> m_clear_event;
};

