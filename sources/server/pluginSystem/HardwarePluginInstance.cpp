#include "stdafx.h"
//TODO : voir pour utiliser la classe CThreadBase en incluant le concept de join dans cette derniere

#include "HardwarePluginInstance.h"
#include "plugins/hardware/IHardwarePlugin.h"
#include "plugins/hardware/HardwarePluginInformation.h"


CHardwarePluginInstance::CHardwarePluginInstance(IHardwarePlugin * plugin) 
    : m_pPlugin(plugin)
{
	if (m_pPlugin != NULL)
	{
		setName("nom du plugin"); //todo : à partir de la base de données
	}
}

void CHardwarePluginInstance::doWork()
{
   if (m_pPlugin != NULL)
	{
      // TODO : we can set protections here (restart plugin if it crashes, force to stop it...)
      try
      {
         m_pPlugin->doWork(m_pluginConfigurationProvider);
      }
      catch (boost::thread_interrupted&)
      {
         // End-of-thread exception was not catch by plugin,
         // it's a developer's error, we have to report it
         BOOST_LOG_TRIVIAL(error) << getName() << " didn't catch boost::thread_interrupted.";
      }
      catch (std::exception& e)
      {
         // Plugin crashed
         BOOST_LOG_TRIVIAL(error) << getName() << " crashed with exception : " << e.what();
      }
      catch (...)
      {
         // Plugin crashed
         BOOST_LOG_TRIVIAL(error) << getName() << " crashed with unknown exception.";
      }
   }
}
