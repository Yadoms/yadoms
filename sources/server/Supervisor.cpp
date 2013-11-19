#include "Supervisor.h"
#include <boost/log/trivial.hpp>
#include "PluginSystem/HardwarePluginInstance.h"


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

        m_hardwarePluginManager.buildPluginFactoryList("plugins"); //todo : gerer le chemin à partir de la conf

        //récupérer la liste des plugins à instancier depuis la base
        CHardwarePluginFactory * pFactory = m_hardwarePluginManager.getFactory("FakePlugin");
        CHardwarePluginInstance fakePlugin(pFactory->construct());

        fakePlugin.start();
        

        CHardwarePluginInstance fakePlugin2(pFactory->construct());
        fakePlugin2.start();


        while(1)
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
        }

	    BOOST_LOG_TRIVIAL(info) << "Supervisor is stopped";
    }
    catch(...)
    {
        BOOST_LOG_TRIVIAL(error) << "Supervisor : unhandled exception.";
    }

    m_hardwarePluginManager.freePluginFactoryList();
}