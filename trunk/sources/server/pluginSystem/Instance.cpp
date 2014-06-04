#include "stdafx.h"

#include "Instance.h"
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>


namespace pluginSystem
{

CInstance::CInstance(
   const boost::shared_ptr<const CFactory> plugin,
   const boost::shared_ptr<database::entities::CPlugin> pluginData,
   boost::shared_ptr<database::IDeviceRequester> deviceRequester,
   boost::shared_ptr<database::IKeywordRequester> keywordRequester,
   boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
   const boost::shared_ptr<IQualifier> qualifier,
   shared::event::CEventHandler& supervisor,
   int pluginManagerEventId)
    : CThreadBase(pluginData->Name()), m_pPlugin(plugin), m_qualifier(qualifier), m_supervisor(supervisor), m_pluginManagerEventId(pluginManagerEventId),
    m_context(new CYadomsApiImplementation(plugin->getInformation(), pluginData, deviceRequester, keywordRequester, acquisitionRequester))
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

      if (getStatus() == kStopping)
      {
         // Normal stop
         return;
      }
      else
      {
         // Plugin has stopped without stop requested
         YADOMS_LOG(error) << getName() << " has stopped itself.";
         m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin stopped itself");
      }
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

   YADOMS_LOG(info) << m_context->getInformation().getName() << " is stopped";
   
   // Signal the abnormal stop
   CManagerEvent event(CManagerEvent::kPluginInstanceAbnormalStopped, m_context->getPluginId(), m_pPlugin->getInformation(), getStatus() == kStopping);
   m_supervisor.postEvent<CManagerEvent>(m_pluginManagerEventId, event);
}

void CInstance::postCommand(const communication::command::CDeviceCommand & message) const
{
   BOOST_ASSERT(m_context);
   // Post event to the plugin
   m_context->getEventHandler().postEvent<communication::command::CDeviceCommand>(shared::plugin::yadomsApi::IYadomsApi::kEventDeviceCommand, message);
}

void CInstance::updateConfiguration(const std::string& newConfiguration) const
{
   BOOST_ASSERT(m_context);
   // Post event to the plugin
   m_context->getEventHandler().postEvent<std::string>(shared::plugin::yadomsApi::IYadomsApi::kEventUpdateConfiguration, newConfiguration);
}

const std::string CInstance::getPluginName() const
{
   return m_pPlugin->getLibraryPath().stem().string();
}

} // namespace pluginSystem
