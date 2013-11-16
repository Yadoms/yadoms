#pragma once

//TODO : voir pour utiliser la classe CThreadBase en incluant le concept de join dans cette derniere

#include "HardwarePluginInstance.h"
#include "../plugins/hardware/IHardwarePlugin.h"


CHardwarePluginInstance::CHardwarePluginInstance(IHardwarePlugin * plugin) : m_pPlugin(plugin)
{
}

void CHardwarePluginInstance::start()
{
	m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&CHardwarePluginInstance::doWork, this)));
}

void CHardwarePluginInstance::stop()
{
    // Stop the thread
    m_thread->interrupt();
    m_thread->join();
    //m_thread->start_thread
    //m_thread->start_thread
    //m_pThread = NULL;

    // Call plugin stop method to free ressources
    //m_pPlugin->stop();
}

void CHardwarePluginInstance::doWork()
{
    // TODO : we can set protections here (restart plugin if it crashes, force to stop it...)
    try 
    {
        // Call plugin main loop
        m_pPlugin->doWork();
    } 
    catch(boost::thread_interrupted&)
    {
        // Thread is stopped
    }
}
