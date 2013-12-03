#include "stdafx.h"

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
         m_pPlugin->doWork(getPluginConfiguration());
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

void CHardwarePluginInstance::updateConfiguration() const
{
   m_pPlugin->updateConfiguration(getPluginConfiguration());
}

std::string CHardwarePluginInstance::getPluginConfiguration() const
{
   // TODO : récupérer la conf de la base à partir du nom du plugin et de l'instance
   std::string fromDatabase = "{\"BoolParameter\":\"false\",\"EnumParameter\":\"12\",\"Serial port\":\"tty3\"}";
   return fromDatabase;
}
