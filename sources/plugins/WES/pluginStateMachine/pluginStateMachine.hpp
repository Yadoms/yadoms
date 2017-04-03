#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include "pluginStateInitialization.hpp"
#include "pluginStateUpdateConfiguration.hpp"
#include "pluginStateDisconnected.hpp"
#include "pluginStateConnected.hpp"
#include "pluginStateTimeOut.hpp"
#include "pluginStateTerminate.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

//--------------------------------------------------------------
/// \brief	Others states
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_STATE((), pluginInitialState);

//--------------------------------------------------------------
/// \brief	Events to pass from one state to another.
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_EVENT(EvtStop);

//--------------------------------------------------------------
/// \brief	Transition table
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_TRANSITION_TABLE((
   //             Start             Event                   Action		       Guard        Next
   //    +--------------------------+--------------------+-----------------------+------------+------------------+
          pluginInitialState        + EvtStart                                                == pluginInitialization,
          pluginInitialization      + EvtInitialized                                          == pluginDisconnected,
          pluginDisconnected        + EvtConnection                                           == pluginConnected,
          pluginConnected           + EvtConnectionLost                                       == pluginTimeOut,
          pluginTimeOut             + EvtDisconnected                                         == pluginDisconnected,

          pluginDisconnected        + EvtNewConfigurationRequested                            == pluginUpdateConfiguration,
          pluginConnected           + EvtNewConfigurationRequested                            == pluginUpdateConfiguration,
          pluginTimeOut             + EvtNewConfigurationRequested                            == pluginUpdateConfiguration,
          pluginUpdateConfiguration + EvtConfigurationUpdated                                 == pluginDisconnected,
          pluginInitialization      + EvtStop                                                 == pluginTerminateState,
          pluginDisconnected        + EvtStop                                                 == pluginTerminateState,
          pluginConnected           + EvtStop                                                 == pluginTerminateState,
          pluginTimeOut             + EvtStop                                                 == pluginTerminateState
   ), plugin_state_table);

BOOST_MSM_EUML_DECLARE_STATE_MACHINE(
(plugin_state_table, 
 init_ << pluginInitialState,
 no_action, // Entry
 no_action, // Exit
 attributes_ << m_factory // Attributes
 ), pluginStateMachine);