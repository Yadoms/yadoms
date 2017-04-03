#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include "deviceStateInitialization.hpp"
#include "deviceStateUpdateConfiguration.hpp"
#include "deviceStateDisconnected.hpp"
#include "deviceStateConnected.hpp"
#include "deviceStateTimeOut.hpp"
#include "deviceStateTerminate.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

//--------------------------------------------------------------
/// \brief	Others states
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_STATE((), InitialState);

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
   //    +--------------------+--------------------+-----------------------+------------+------------------+
          InitialState        + EvtStart                                                == Initialization,
          Initialization      + EvtInitialized                                          == Disconnected,
          Disconnected        + EvtConnection                                           == Connected,
          Connected           + EvtConnectionLost                                       == TimeOut,
          TimeOut             + EvtDisconnected                                         == Disconnected,

          Disconnected        + EvtNewConfigurationRequested                            == UpdateConfiguration,
          Connected           + EvtNewConfigurationRequested                            == UpdateConfiguration,
          TimeOut             + EvtNewConfigurationRequested                            == UpdateConfiguration,
          UpdateConfiguration + EvtConfigurationUpdated                                 == Disconnected,
          Initialization      + EvtStop                                                 == TerminateState,
          Disconnected        + EvtStop                                                 == TerminateState,
          Connected           + EvtStop                                                 == TerminateState,
          TimeOut             + EvtStop                                                 == TerminateState
   ), plugin_state_table);

BOOST_MSM_EUML_DECLARE_STATE_MACHINE(
(plugin_state_table, 
 init_ << InitialState
 ), deviceWES);