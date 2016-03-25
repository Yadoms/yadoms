#include "stdafx.h"

#include "Instance.h"
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/Log.h>
#include <shared/DynamicLibrary.h>

namespace pluginSystem
{

CInstance::CInstance(
   const boost::shared_ptr<const ILibrary> plugin,
   boost::shared_ptr<const database::entities::CPlugin> pluginData,
   boost::shared_ptr<database::IDataProvider> dataProvider,
   boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
   boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
   const boost::shared_ptr<IQualifier> qualifier,
   boost::shared_ptr<shared::event::CEventHandler> supervisor,
   int pluginManagerEventId)
   : m_PluginLibrary(plugin),
    m_pluginData(pluginData),
    m_dataProvider(dataProvider),
    m_deviceManager(deviceManager),
    m_acquisitionHistorizer(acquisitionHistorizer),
    m_qualifier(qualifier),
    m_supervisor(supervisor),
    m_pluginManagerEventId(pluginManagerEventId)
{
   start();
}

CInstance::~CInstance()
{
}

void CInstance::start()
{
   if (!!m_runner)
   {
      YADOMS_LOG(warning) << "Can not start plugin " << m_pluginData->DisplayName() << " : already started";
      return;
   }

   m_pPluginInstance.reset(m_PluginLibrary->construct());

   boost::shared_ptr<shared::process::ILogger> scriptLogger = m_factory->createProcessLogger(); // TODO m_factory est l'équivalent du automation::script::IManager

   boost::shared_ptr<CYPluginApiImplementation> yPluginApi(boost::make_shared<CYPluginApiImplementation>( //TODO déplacer la construction dans m_factory
      m_PluginLibrary->getInformation(),
      m_PluginLibrary->getLibraryPath(),
      m_pluginData,
      m_dataProvider,
      m_deviceManager,
      m_acquisitionHistorizer));

   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi = m_scriptManager->createScriptContext(scriptLogger);
   boost::shared_ptr<shared::script::IStopNotifier> stopNotifier = m_scriptManager->createStopNotifier(m_ruleStateHandler, m_ruleData->Id());

   m_runner = m_scriptManager->createScriptRunner(scriptProperties, scriptLogger, yScriptApi, stopNotifier);
}

void CInstance::requestStop()
{
   m_runner->requestStop();
}

void CInstance::doWork()
{
   BOOST_ASSERT(m_pPluginInstance);
   YADOMS_LOG_CONFIGURE(getName());
   YADOMS_LOG(debug) << m_context->getInformation().getType() << " is starting...";

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

   YADOMS_LOG(information) << m_context->getInformation().getType() << " is stopped";

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
