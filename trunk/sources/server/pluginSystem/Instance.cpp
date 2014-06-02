#include "stdafx.h"

#include "Instance.h"
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>


namespace pluginSystem
{

CInstance::CInstance(
   const boost::shared_ptr<const CFactory> plugin,
   const boost::shared_ptr<database::entities::CPlugin> pluginData,
   const boost::shared_ptr<IQualifier> qualifier,
   shared::event::CEventHandler& supervisor,
   int pluginManagerEventId, 
   boost::asio::io_service& pluginIOService)
    : CThreadBase(pluginData->Name()), m_pPlugin(plugin), m_qualifier(qualifier), m_supervisor(supervisor), m_pluginManagerEventId(pluginManagerEventId),
    m_api(new CYadomsApiImplementation(pluginData, pluginIOService))
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

   try
   {
      // Execute plugin code
      m_pPluginInstance->doWork(m_api);

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

   // Signal the abnormal stop
   CManagerEvent event(CManagerEvent::kPluginInstanceAbnormalStopped, m_api->getInstanceId(), m_pPlugin->getInformation(), getStatus() == kStopping);
   m_supervisor.sendEvent<CManagerEvent>(m_pluginManagerEventId, event);
}

void CInstance::updateConfiguration(const std::string& newConfiguration) const
{
   BOOST_ASSERT(m_pPluginInstance);

   try
   {
      m_pPluginInstance->updateConfiguration(newConfiguration);
   }
   catch (std::exception& e)
   {
      // Plugin crashed
      YADOMS_LOG(error) << getName() << " crashed in updateConfiguration with exception : " << e.what();
      m_qualifier->signalCrash(m_pPlugin->getInformation(), std::string("Plugin crashed in doWork with exception : ") + e.what());
   }
   catch (...)
   {
      // Plugin crashed
      YADOMS_LOG(error) << getName() << " crashed in updateConfiguration with unknown exception.";
      m_qualifier->signalCrash(m_pPlugin->getInformation(), "Plugin crashed in doWork with unknown exception");
   }
}

const std::string CInstance::getPluginName() const
{
   return m_pPlugin->getLibraryPath().stem().string();
}

} // namespace pluginSystem
