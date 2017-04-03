#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/DataContainer.h>

#include "WESConfiguration.h"
#include "WESFactory.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

//--------------------------------------------------------------
/// \brief	attributes déclaration, parameters to pass to states or events
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_DECLARE_ATTRIBUTE(boost::shared_ptr<yApi::IYPluginApi>         , m_pluginApi);
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(boost::shared_ptr<CWESConfiguration>         , m_configuration);
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(boost::shared_ptr<shared::event::CEventTimer>, m_refreshTimer);
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(shared::CDataContainer                       , m_newConfiguration);

BOOST_MSM_EUML_ATTRIBUTES((attributes_ << m_pluginApi), 
                          parameters_attributes);

BOOST_MSM_EUML_ATTRIBUTES((attributes_ << m_pluginApi << m_newConfiguration), 
                          newConfiguration_attributes);

//--------------------------------------------------------------
/// \brief	Events with attributes.
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EvtStart                    , parameters_attributes);
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EvtConnection               , parameters_attributes);
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EvtInitialized              , parameters_attributes);
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EvtConnectionLost           , parameters_attributes);
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EvtDisconnected             , parameters_attributes);
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EvtConfigurationUpdated     , parameters_attributes);
BOOST_MSM_EUML_EVENT_WITH_ATTRIBUTES(EvtNewConfigurationRequested, newConfiguration_attributes);