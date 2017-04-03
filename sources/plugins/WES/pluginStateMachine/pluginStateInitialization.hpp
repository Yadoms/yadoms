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
/// \brief	attributes déclaration, parameters to pass to states or events
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_DECLARE_ATTRIBUTE(boost::shared_ptr<CWESFactory>      , m_factory);

//--------------------------------------------------------------
/// \brief	Déclaration de l'état initialization
/// \note   
//--------------------------------------------------------------

BOOST_MSM_EUML_ACTION(pluginInitializationEntryState)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm& stateMachine, State&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      auto configuration = pluginInitialization.get_attribute(m_configuration);
      auto factory = pluginInitialization.get_attribute(m_factory);

      api->setPluginState(yApi::historization::EPluginState::kCustom, "initialization");
      factory = boost::make_shared<CWESFactory>(api, configuration);

      stateMachine.process_event(EvtInitialized(api));
   }

   template <class Evt, class Fsm, class State>
   void exception_caught(Evt const& evt, Fsm&, std::exception&)
   {
      auto api = evt.get_attribute(m_pluginApi);

      api->setPluginState(yApi::historization::EPluginState::kCustom, "initializationError");
      YADOMS_LOG(error) << "WES plugin initialization error...";
   }
};

BOOST_MSM_EUML_STATE((pluginInitializationEntryState,
                      no_action,
                      attributes_ << m_configuration << m_factory,
                      configure_ << no_configure_),
                     pluginInitialization);