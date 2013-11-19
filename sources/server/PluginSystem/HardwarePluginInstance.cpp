//TODO : voir pour utiliser la classe CThreadBase en incluant le concept de join dans cette derniere

#include "HardwarePluginInstance.h"
#include "../plugins/hardware/IHardwarePlugin.h"


CHardwarePluginInstance::CHardwarePluginInstance(plugins::IHardwarePlugin * plugin) 
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
        m_pPlugin->doWork();
    }
}
