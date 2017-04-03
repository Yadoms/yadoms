#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/DataContainer.h>

#include "../stateCommonDeclaration.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

//--------------------------------------------------------------
/// \brief	Déclaration de l'état initialization
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_ACTION(pluginTerminateExitState)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm& stateMachine, State&)
   {
      auto api = pluginTerminateState.get_attribute(m_pluginApi);

      YADOMS_LOG(information) << "Stop requested";
      api->setPluginState(yApi::historization::EPluginState::kStopped);
   }
};

BOOST_MSM_EUML_TERMINATE_STATE((no_action, 
                                pluginTerminateExitState,
                                attributes_ << m_pluginApi,
                                configure_ << no_configure_),
                                pluginTerminateState);