#include "Supervisor.h"
#include <boost/log/trivial.hpp>


CSupervisor::CSupervisor(void)
	:CThreadBase("Supervisor")
{
}


CSupervisor::~CSupervisor(void)
{
}

void CSupervisor::doWork()
{
    try
    {
	    BOOST_LOG_TRIVIAL(info) << "Supervisor is starting";

        m_hardwarePluginManager.buildPluginFactoryList("plugins"); //todo : gerer le chemin � partir de la conf

        //r�cup�rer la liste des plugins � instancier depuis la base
        

	    BOOST_LOG_TRIVIAL(info) << "Supervisor is stopped";
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << "Supervisor : unhandled exception.";
    }

    m_hardwarePluginManager.freePluginFactoryList();
}