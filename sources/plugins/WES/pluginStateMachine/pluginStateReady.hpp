#pragma once
#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/Log.h>

#include "pluginStateCommonDeclaration.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

//--------------------------------------------------------------
/// \brief	Time Out State
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_ACTION(pluginReadyEntryState)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm&, State&)
   {
      auto api = evt.get_attribute(m_pluginApi);

      YADOMS_LOG(error) << "Plugin Ready : Waiting for the creation of a device";
      api->setPluginState(yApi::historization::EPluginState::kCustom, "ready");
   }
};

BOOST_MSM_EUML_STATE((pluginReadyEntryState,
                      no_action,
                      attributes_ << no_attributes_,
                      configure_ << no_configure_),
                     pluginReady);