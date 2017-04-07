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

BOOST_MSM_EUML_ACTION(pluginFaultyEntryState)
{
   template <class Evt, class Fsm, class State>
   void operator()(Evt const& evt, Fsm& stateMachine, State&)
   {
      auto api = evt.get_attribute(m_pluginApi);
      auto factory = evt.get_attribute(m_factory);
      auto configuration = pluginFaulty.get_attribute(m_configuration);
      auto refreshTimer = pluginFaulty.get_attribute(m_refreshTimer);

      api->setPluginState(yApi::historization::EPluginState::kCustom, "faulty");

      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
      {
         YADOMS_LOG(information) << "Stop requested";
         api->setPluginState(yApi::historization::EPluginState::kStopped);
         stateMachine.stop();
         return;
      }
      case kConnectionRetryTimer:
         break;
      case yApi::IYPluginApi::kEventUpdateConfiguration:
      {
         stateMachine.enqueue_event(EvtNewConfigurationRequested(api, api->getEventHandler().getEventData<shared::CDataContainer>(), factory));
         break;
      }
      case kRefreshStatesReceived:
      {
         YADOMS_LOG(trace) << "Timer received";

         auto forceRefresh = false;

         try { forceRefresh = api->getEventHandler().getEventData<bool>(); }
         catch (shared::exception::CBadConversion&) {}

         // TODO : device list to parse and update

         stateMachine.enqueue_event(EvtConnection(api, factory));
         break;
      }
      case yApi::IYPluginApi::kEventManuallyDeviceCreation:
      {
         // Yadoms asks for device creation
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IManuallyDeviceCreationRequest>>();
         YADOMS_LOG(information) << "Manually device creation request received for device : " << request->getData().getDeviceName();
         try
         {
            // Creation of the device
            request->sendSuccess(factory->createDeviceManually(api, request->getData()));
            stateMachine.process_event(EvtConfigurationUpdated(api, factory));
         }
         catch (CManuallyDeviceCreationException& e)
         {
            request->sendError(e.what());
         }
         catch (std::exception &e)
         {
            YADOMS_LOG(information) << "Unknow error : " << e.what();
         }
         break;
      }
      default:
      {
         YADOMS_LOG(information) << "Unknown message id for pluginStateFaulty";
         break;
      }
      }
   }

   template <class Evt, class Fsm, class State>
   void exception_caught(Evt const& evt, Fsm&, std::exception& exception)
   {
      auto api = evt.get_attribute(m_pluginApi);
      auto refreshTimer = pluginFaulty.get_attribute(m_refreshTimer);

      refreshTimer->stop();
      api->getEventHandler().createTimer(kConnectionRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
   }
};

BOOST_MSM_EUML_STATE((pluginFaultyEntryState,
                      no_action,
                      attributes_ << m_configuration << m_refreshTimer,
                      configure_ << no_configure_),
                     pluginFaulty);