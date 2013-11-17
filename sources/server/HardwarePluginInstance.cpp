#pragma once

//TODO : voir pour utiliser la classe CThreadBase en incluant le concept de join dans cette derniere

#include "HardwarePluginInstance.h"
#include "../plugins/hardware/IHardwarePlugin.h"


CHardwarePluginInstance::CHardwarePluginInstance(IHardwarePlugin * plugin) : m_pPlugin(plugin)
{
	if (m_pPlugin != NULL)
	{
		setName(m_pPlugin->tostring());
	}
}

void CHardwarePluginInstance::doWork()
{
    // TODO : we can set protections here (restart plugin if it crashes, force to stop it...)
    m_pPlugin->doWork();
}
