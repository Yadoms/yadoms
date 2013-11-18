#pragma once

#include "HardwarePluginManager.h"

CHardwarePluginManager::CHardwarePluginManager()
{
}

CHardwarePluginManager::~CHardwarePluginManager()
{
    // m_PluginInstances should be emptied by call to StopPlugins
    BOOST_ASSERT(m_pluginFactories.empty());
}


// Returns all plugin libraries installed
std::list<std::string> CHardwarePluginManager::findAvalaiblePlugins(const std::string & initialDir)
{
    // Parse the hardware plugin directory to find all plugin libraries
    // TODO
    std::list<std::string> result;
    return result;
}


void CHardwarePluginManager::buildPluginFactoryList(const std::string & initialDir)
{
    std::list<std::string> avalaiblePluginFileNames = findAvalaiblePlugins(initialDir);

    for (std::list<std::string>::const_iterator libNameIt = avalaiblePluginFileNames.begin(); libNameIt != avalaiblePluginFileNames.end() ; ++libNameIt)
    {
        // Load the library
        CPluginFactory<plugins::IHardwarePlugin, plugins::CHardwarePluginInformation> * newFactory = new CPluginFactory<plugins::IHardwarePlugin, plugins::CHardwarePluginInformation>();
        if (!newFactory->load(*libNameIt))
        {
            delete newFactory;
            continue;
        }
        m_pluginFactories.push_back(newFactory);
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

CPluginFactory<plugins::IHardwarePlugin, plugins::CHardwarePluginInformation> * CHardwarePluginManager::getFactory(const std::string & pluginName)
{
    PluginFactoryList::iterator itFind;
    
    for(itFind = m_pluginFactories.begin(); itFind != m_pluginFactories.end(); itFind++)
    {
        if((*itFind)->getInfo()->getName() == pluginName)
        {
            return (*itFind);
        }
    }
    return NULL;
}


