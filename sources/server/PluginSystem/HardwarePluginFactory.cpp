
#include "HardwarePluginFactory.h"

CHardwarePluginFactory::CHardwarePluginFactory()
{
}

CHardwarePluginFactory::~CHardwarePluginFactory()
{
}


bool CHardwarePluginFactory::load(const std::string & libraryFile)
{
    //load the plugin file
    bool result = CPluginFactory<plugins::IHardwarePlugin>::load(libraryFile);
    if(result)
    {
        //load additional exported methods
        m_getInfo = (plugins::CHardwarePluginInformation* (*)(void))GetFunctionPointer("getInfo");

        //check if all methods are loaded
        if(m_getInfo == NULL)
            result = false;
    }

    return result;
}


plugins::CHardwarePluginInformation * CHardwarePluginFactory::getInfo()
{
    if(m_getInfo != NULL)
        return m_getInfo();   
    return NULL;
}
