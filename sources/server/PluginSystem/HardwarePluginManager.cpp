#include "HardwarePluginManager.h"

CHardwarePluginManager::CHardwarePluginManager()
{
}

CHardwarePluginManager::~CHardwarePluginManager()
{
    // m_pluginFactories should be emptied by call to StopPlugins
    BOOST_ASSERT(m_pluginFactories.empty());
}


// Returns all plugin libraries installed
std::list<std::string> CHardwarePluginManager::findAvalaiblePlugins(const std::string & initialDir)
{
    // Parse the hardware plugin directory to find all plugin libraries
    // TODO
    std::list<std::string> result;

    result.push_back("../lib/Debug/fakePlugin.dll");

    return result;
}


void CHardwarePluginManager::buildPluginFactoryList(const std::string & initialDir)
{
    //search for library files
    std::list<std::string> avalaiblePluginFileNames = findAvalaiblePlugins(initialDir);

    for (std::list<std::string>::const_iterator libNameIt = avalaiblePluginFileNames.begin(); libNameIt != avalaiblePluginFileNames.end() ; ++libNameIt)
    {
        // generate factory for current found plugin
        CHardwarePluginFactory *pNewFactory = new CHardwarePluginFactory();
        if (!pNewFactory->load(*libNameIt))
        {
            delete pNewFactory;
            continue;
        }
        m_pluginFactories.push_back(pNewFactory);
    }
}

void CHardwarePluginManager::freePluginFactoryList()
{
    // Free the libraries
    while (!m_pluginFactories.empty())
    {
        m_pluginFactories.front()->unload();
        delete m_pluginFactories.front();
        m_pluginFactories.pop_front();         
    }
}

CHardwarePluginFactory * CHardwarePluginManager::getFactory(const std::string & pluginName)
{
    std::list<CHardwarePluginFactory *>::iterator itFind;
    
    for(itFind = m_pluginFactories.begin(); itFind != m_pluginFactories.end(); itFind++)
    {
        if((*itFind)->getInfo()->getName() == pluginName)
        {
            return (*itFind);
        }
    }
    return NULL;
}


