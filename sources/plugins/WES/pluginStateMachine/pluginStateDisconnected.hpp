#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Log.h>

#include "../stateCommonDeclaration.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

//--------------------------------------------------------------
/// \brief	Déclaration de l'état initialization
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_ACTION(pluginDisconnectedEntryState)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm& stateMachine, State&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      auto configuration = pluginDisconnected.get_attribute(m_configuration);
      auto refreshTimer = pluginDisconnected.get_attribute(m_refreshTimer);
      //auto factory = Initialization.get_attribute(m_factory);

      YADOMS_LOG(information) << "Init the connexion ...";
      api->setPluginState(yApi::historization::EPluginState::kCustom, "noConnection");

      //m_ioManager->readAllIOFromDevice(api, true);

      // Timer used to read periodically IOs from the WES
      refreshTimer = api->getEventHandler().createTimer(kRefreshStatesReceived, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      stateMachine.process_event(EvtConnection(api));
   }

   template <class Evt, class Fsm, class State>
   void exception_caught(Evt const& evt, Fsm&, std::exception&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
   }
};

BOOST_MSM_EUML_STATE((pluginDisconnectedEntryState,
                      no_action,
                      attributes_ << m_configuration << m_refreshTimer,
                      configure_ << no_configure_),
                     pluginDisconnected);