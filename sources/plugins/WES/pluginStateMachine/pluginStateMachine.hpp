#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include "pluginStateInitialization.hpp"
#include "pluginStateUpdateConfiguration.hpp"
#include "pluginStateFaulty.hpp"
#include "pluginStateConnected.hpp"
#include "pluginStateReady.hpp"
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
/// \brief	Transition table
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_TRANSITION_TABLE((
   //             Start             Event                   Action		       Guard        Next
   //    +--------------------------+--------------------+-----------------------+------------+------------------+
          pluginInitialState        + EvtStart                                                == pluginInitialization,
          pluginInitialization      + EvtInitialized                                          == pluginFaulty,
          pluginInitialization      + EvtInitializedNoEquipment                               == pluginReady,
          pluginReady               + EvtConfigurationUpdated                                 == pluginFaulty,
          pluginFaulty              + EvtConnection                                           == pluginConnected,

          pluginFaulty              + EvtNewConfigurationRequested                            == pluginUpdateConfiguration,
          pluginConnected           + EvtNewConfigurationRequested                            == pluginUpdateConfiguration,
          pluginReady               + EvtNewConfigurationRequested                            == pluginUpdateConfiguration,
          pluginUpdateConfiguration + EvtConfigurationUpdated                                 == pluginFaulty,
          pluginInitialization      + EvtStop                                                 == pluginTerminateState,
          pluginFaulty              + EvtStop                                                 == pluginTerminateState,
          pluginConnected           + EvtStop                                                 == pluginTerminateState
   ), plugin_state_table);

BOOST_MSM_EUML_DECLARE_STATE_MACHINE(
(plugin_state_table, 
 init_ << pluginInitialState/*,
 no_action, // Entry
 no_action, // Exit
 attributes_ << no_attributes_*/ /*m_factory*/ // Attributes
 ), pluginStateMachine);