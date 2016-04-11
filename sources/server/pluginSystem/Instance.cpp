#include "stdafx.h"
#include "Instance.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace pluginSystem
{
   CInstance::CInstance(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                        boost::shared_ptr<shared::process::IProcess> process,
                        boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> yPluginApi)
      : m_pluginInformation(pluginInformation),
        m_process(process),
        m_yPluginApi(yPluginApi)
   {
   }

   CInstance::~CInstance()
   {
   }

   void CInstance::requestStop()
   {
      postStopRequest();
   }

   void CInstance::kill()
   {
      m_process->kill();
   }

   void CInstance::postStopRequest() const
   {
      // Post event to the plugin
      m_yPluginApi->getEventHandler().postEvent(shared::plugin::yPluginApi::IYPluginApi::kEventStopRequested);
   }

   //TODO
   //void CInstance::doWork()
   //{
   //   BOOST_ASSERT(m_pPluginInstance);
   //   YADOMS_LOG_CONFIGURE(getName());
   //   YADOMS_LOG(debug) << m_context->getInformation().getType() << " is starting...";
   //
   //   try
   //   {
   //      // Execute plugin code
   //      m_pPluginInstance->doWork(m_context);
   //
   //      if (isStopping())
   //      {
   //         // Normal stop
   //         return;
   //      }
   //
   //      // Plugin has stopped without stop requested
   //      YADOMS_LOG(error) << getName() << " has stopped itself.";
   //      m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin stopped itself");
   //   }
   //   catch (boost::thread_interrupted&)
   //   {
   //      // End-of-thread exception was not catch by plugin,
   //      // it's a developer's error, we have to report it
   //      YADOMS_LOG(error) << getName() << " didn't catch boost::thread_interrupted.";
   //      m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin didn't catch boost::thread_interrupted");
   //   }
   //   catch (std::exception& e)
   //   {
   //      // Plugin crashed
   //      YADOMS_LOG(error) << getName() << " crashed in doWork with exception : " << e.what();
   //      m_qualifier->signalCrash(m_pPlugin->getInformation(), std::string("Plugin crashed in doWork with exception : ") + e.what());
   //   }
   //   catch (...)
   //   {
   //      // Plugin crashed
   //
   //      YADOMS_LOG(error) << getName() << " crashed in doWork with unknown exception.";
   //      m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin crashed in doWork with unknown exception");
   //   }
   //
   //   YADOMS_LOG(information) << m_context->getInformation().getType() << " is stopped";
   //
   //   // Signal the abnormal stop
   //   CManagerEvent event(CManagerEvent::kPluginInstanceAbnormalStopped, m_context->getPluginId(), m_pPlugin->getInformation(), isStopping());
   //   m_supervisor->postEvent<CManagerEvent>(m_pluginManagerEventId, event);
   //}
   //
   //void CInstance::postCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) const
   //{
   //   BOOST_ASSERT(m_context);
   //   // Post event to the plugin
   //   m_context->getEventHandler().postEvent<boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> >(shared::plugin::yPluginApi::IYPluginApi::kEventDeviceCommand, command);
   //}
   //
   //void CInstance::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> & request) const
   //{
   //   BOOST_ASSERT(m_context);
   //   // Post event to the plugin
   //   m_context->getEventHandler().postEvent< boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> >(shared::plugin::yPluginApi::IYPluginApi::kEventManuallyDeviceCreation, request);
   //}
   //
   //void CInstance::postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> & request) const
   //{
   //	BOOST_ASSERT(m_context);
   //	// Post event to the plugin
   //   m_context->getEventHandler().postEvent< boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> >(shared::plugin::yPluginApi::IYPluginApi::kBindingQuery, request);
   //}

   void CInstance::updateConfiguration(const shared::CDataContainer& newConfiguration) const
   {
      //TODO
      //BOOST_ASSERT(m_context);
      //// Post event to the plugin
      //m_context->getEventHandler().postEvent<shared::CDataContainer>(shared::plugin::yPluginApi::IYPluginApi::kEventUpdateConfiguration, newConfiguration);
   }

   std::string CInstance::getPluginName() const
   {
      return m_pluginInformation->getType();
   }
} // namespace pluginSystem
