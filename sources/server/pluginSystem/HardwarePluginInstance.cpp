#include "stdafx.h"

#include "HardwarePluginInstance.h"
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>


CHardwarePluginInstance::CHardwarePluginInstance(
   const boost::shared_ptr<const CHardwarePluginFactory> plugin,
   const boost::shared_ptr<server::database::entities::CHardware> context,
   const boost::shared_ptr<IHardwarePluginQualifier> qualifier,
   shared::event::CEventHandler& supervisor,
   int pluginManagerEventId, 
   boost::asio::io_service * pluginIOService)
    : CThreadBase(context->getName()), m_pPlugin(plugin), m_context(context), m_qualifier(qualifier), m_supervisor(supervisor), m_pluginManagerEventId(pluginManagerEventId), m_pluginIOService(pluginIOService)
{
	BOOST_ASSERT(m_pPlugin);
   m_pPluginInstance.reset(m_pPlugin->construct());
   
   start();
}

CHardwarePluginInstance::~CHardwarePluginInstance()
{
   stop();
}

void CHardwarePluginInstance::doWork()
{
   BOOST_ASSERT(m_pPluginInstance);
   YADOMS_LOG_CONFIGURE(getName());

   try
   {
      // Execute plugin code
      m_pPluginInstance->doWork(m_context->getConfiguration(), m_pluginIOService);

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
   CHardwarePluginManagerEvent event(CHardwarePluginManagerEvent::kPluginInstanceAbnormalStopped, m_context->getId(), m_pPlugin->getInformation(), getStatus() == kStopping);
   m_supervisor.sendEvent<CHardwarePluginManagerEvent>(m_pluginManagerEventId, event);
}

void CHardwarePluginInstance::updateConfiguration(const std::string& newConfiguration) const
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

const std::string CHardwarePluginInstance::getPluginName() const
{
   return m_pPlugin->getLibraryPath().stem().string();
}
