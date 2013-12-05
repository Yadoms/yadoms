#include "stdafx.h"

#include "HardwarePluginInstance.h"
#include "plugins/hardware/IHardwarePlugin.h"
#include "plugins/hardware/HardwarePluginInformation.h"


CHardwarePluginInstance::CHardwarePluginInstance(const std::string& pluginInstanceName, boost::shared_ptr<IHardwarePlugin> pluginInstance) 
    : CThreadBase(pluginInstanceName), m_pPluginInstance(pluginInstance)
{
	BOOST_ASSERT(m_pPluginInstance);
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
      m_pPluginInstance->doWork(getPluginConfiguration());
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

void CHardwarePluginInstance::updateConfiguration() const
{
   BOOST_ASSERT(m_pPluginInstance);
   m_pPluginInstance->updateConfiguration(getPluginConfiguration());
}

std::string CHardwarePluginInstance::getPluginConfiguration() const
{
   // TODO : récupérer la conf de la base à partir du nom du plugin et de l'instance
   std::string fromDatabase = "{\"BoolParameter\":\"false\",\"EnumParameter\":\"12\",\"Serial port\":\"tty3\"}";
   return fromDatabase;
}
