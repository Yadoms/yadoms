#include "stdafx.h"

#include "Instance.h"
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/Log.h>
#include <shared/DynamicLibrary.h>

namespace pluginSystem
{

CInstance::CInstance(
   const boost::shared_ptr<const IFactory> plugin,
   const boost::shared_ptr<const database::entities::CPlugin> pluginData,
   boost::shared_ptr<database::IPluginEventLoggerRequester> pluginEventLoggerRequester,
   boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
   boost::shared_ptr<database::IKeywordRequester> keywordRequester,
   boost::shared_ptr<database::IRecipientRequester> recipientRequester,
   boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
   boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
   const boost::shared_ptr<IQualifier> qualifier,
   boost::shared_ptr<shared::event::CEventHandler> supervisor,
   int pluginManagerEventId)
    : CThreadBase(pluginData->Name()), m_pPlugin(plugin), m_qualifier(qualifier), m_supervisor(supervisor), m_pluginManagerEventId(pluginManagerEventId),
    m_context(new CYPluginApiImplementation(plugin->getInformation(), m_pPlugin->getLibraryPath(), pluginData, pluginEventLoggerRequester, deviceManager, keywordRequester, recipientRequester, acquisitionRequester, acquisitionHistorizer))
{
	BOOST_ASSERT(m_pPlugin);
   m_pPluginInstance.reset(m_pPlugin->construct());

   start();
}

CInstance::~CInstance()
{
   stop();
}

void CInstance::doWork()
{
   BOOST_ASSERT(m_pPluginInstance);
   YADOMS_LOG_CONFIGURE(getName());
   YADOMS_LOG(debug) << m_context->getInformation().getName() << " is starting...";

   try
   {
      // Execute plugin code
      m_pPluginInstance->doWork(m_context);

      if (isStopping())
      {
         // Normal stop
         return;
      }

      // Plugin has stopped without stop requested
      YADOMS_LOG(error) << getName() << " has stopped itself.";
      m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin stopped itself");
   }
   catch (boost::thread_interrupted&)
   {
      // End-of-thread exception was not catch by plugin,
      // it's a developer's error, we have to report it
      YADOMS_LOG(error) << getName() << " didn't catch boost::thread_interrupted.";
      m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin didn't catch boost::thread_interrupted");
   }
   catch (std::exception& e)
   {
      // Plugin crashed
      YADOMS_LOG(error) << getName() << " crashed in doWork with exception : " << e.what();
      m_qualifier->signalCrash(m_pPlugin->getInformation(), std::string("Plugin crashed in doWork with exception : ") + e.what());
   }
   catch (...)
   {
      // Plugin crashed

      YADOMS_LOG(error) << getName() << " crashed in doWork with unknown exception.";
      m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin crashed in doWork with unknown exception");
   }

   YADOMS_LOG(information) << m_context->getInformation().getName() << " is stopped";

   // Signal the abnormal stop
   CManagerEvent event(CManagerEvent::kPluginInstanceAbnormalStopped, m_context->getPluginId(), m_pPlugin->getInformation(), isStopping());
   m_supervisor->postEvent<CManagerEvent>(m_pluginManagerEventId, event);
}

void CInstance::postCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) const
{
   BOOST_ASSERT(m_context);
   // Post event to the plugin
   m_context->getEventHandler().postEvent<boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> >(shared::plugin::yPluginApi::IYPluginApi::kEventDeviceCommand, command);
}

void CInstance::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> & request) const
{
   BOOST_ASSERT(m_context);
   // Post event to the plugin
   m_context->getEventHandler().postEvent< boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> >(shared::plugin::yPluginApi::IYPluginApi::kEventManuallyDeviceCreation, request);
}

void CInstance::postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> & request) const
{
	BOOST_ASSERT(m_context);
	// Post event to the plugin
   m_context->getEventHandler().postEvent< boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> >(shared::plugin::yPluginApi::IYPluginApi::kBindingQuery, request);
}

void CInstance::updateConfiguration(const shared::CDataContainer & newConfiguration) const
{
   BOOST_ASSERT(m_context);
   // Post event to the plugin
   m_context->getEventHandler().postEvent<shared::CDataContainer>(shared::plugin::yPluginApi::IYPluginApi::kEventUpdateConfiguration, newConfiguration);
}

std::string CInstance::getPluginName() const
{
   return shared::CDynamicLibrary::ToLibName(m_pPlugin->getLibraryPath().string());
}

} // namespace pluginSystem
