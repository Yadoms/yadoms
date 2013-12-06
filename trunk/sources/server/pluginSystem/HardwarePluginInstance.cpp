#include "stdafx.h"

#include "HardwarePluginInstance.h"
#include "plugins/hardware/IHardwarePlugin.h"
#include "plugins/hardware/HardwarePluginInformation.h"


CHardwarePluginInstance::CHardwarePluginInstance(
   boost::shared_ptr<const CHardwarePluginFactory> plugin,
   const boost::shared_ptr<CHardware> context)
    : CThreadBase(context->getName()), m_pPlugin(plugin), m_context(context)
{
	BOOST_ASSERT(m_pPlugin);
   m_pPluginInstance.reset(m_pPlugin->construct());
}

CHardwarePluginInstance::~CHardwarePluginInstance()
{
   stop();
}

void CHardwarePluginInstance::doWork()
{
   BOOST_ASSERT(m_pPluginInstance);
   YADOMS_LOG_CONFIGURE(getName());

   // TODO : we can set protections here (restart plugin if it crashes, force to stop it...)
   try
   {
      m_pPluginInstance->doWork(m_context->getConfiguration());
   }
   catch (boost::thread_interrupted&)
   {
      // End-of-thread exception was not catch by plugin,
      // it's a developer's error, we have to report it
      YADOMS_LOG(error) << getName() << " didn't catch boost::thread_interrupted.";
   }
   catch (std::exception& e)
   {
      // Plugin crashed
      YADOMS_LOG(error) << getName() << " crashed with exception : " << e.what();
   }
   catch (...)
   {
      // Plugin crashed
      YADOMS_LOG(error) << getName() << " crashed with unknown exception.";
   }
}

void CHardwarePluginInstance::updateConfiguration(const std::string& newConfiguration) const
{
   BOOST_ASSERT(m_pPluginInstance);
   m_pPluginInstance->updateConfiguration(newConfiguration);
}
