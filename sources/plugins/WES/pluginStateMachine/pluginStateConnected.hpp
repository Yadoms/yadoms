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
/// \brief	Connected state for plugin Machine
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_ACTION(PluginConnectedEntryState)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm&, State&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      api->setPluginState(yApi::historization::EPluginState::kRunning);
   }

   template <class Evt, class Fsm, class State>
   void exception_caught(Evt const& evt, Fsm&, std::exception&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      api->setPluginState(yApi::historization::EPluginState::kCustom, "noConnection");
      api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
   }
};

BOOST_MSM_EUML_STATE((PluginConnectedEntryState,
                      no_action),
                     pluginConnected);