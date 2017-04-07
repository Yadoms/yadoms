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
/// \brief	Déclaration de l'état initialization
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_ACTION(pluginUpdateConfigurationEntryState)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm& stateMachine, State&)
   {
      auto configuration = pluginUpdateConfiguration.get_attribute(m_configuration);
      auto api = evt.get_attribute(m_pluginApi);
      auto newConfiguration = evt.get_attribute(m_newConfiguration);
      auto factory = evt.get_attribute(m_factory);

      api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
      YADOMS_LOG(information) << "Update Configuration ...";
      BOOST_ASSERT(!newConfiguration.empty());

      if (!configuration)
         configuration = boost::make_shared<CWESConfiguration>();

      // Update configuration
      configuration->initializeWith(newConfiguration);
      stateMachine.enqueue_event(EvtConfigurationUpdated(api, factory));
   } 

   template <class Evt, class Fsm, class State>
   void exception_caught(Evt const& evt, Fsm&, std::exception&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");

      YADOMS_LOG(information) << "Wrong configuration update";
   }
};

BOOST_MSM_EUML_STATE((pluginUpdateConfigurationEntryState,
                      no_action,
                      attributes_ << m_configuration,
                      configure_ << no_configure_),
                     pluginUpdateConfiguration);