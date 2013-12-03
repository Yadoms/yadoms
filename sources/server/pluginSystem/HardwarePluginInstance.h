//
// HardwarePluginInstance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include "tools/ThreadBase.h"
#include "plugins/hardware/IHardwarePlugin.h"

//--------------------------------------------------------------
/// \brief	this class is used to manage a plugin instance. 
//--------------------------------------------------------------
class CHardwarePluginInstance : public CThreadBase
{
public:
    //--------------------------------------------------------------
    /// \brief	Constructor
    /// \param [in]	plugin: the plugin instance to handle
    //--------------------------------------------------------------
    CHardwarePluginInstance(IHardwarePlugin * plugin);

    //--------------------------------------------------------------
    /// \brief			Notify the plugin about its configuration changed
    //--------------------------------------------------------------
    virtual void updateConfiguration() const;

protected:
    //--------------------------------------------------------------
    /// \brief			The main plugin work
    //--------------------------------------------------------------
    virtual void doWork();

private:
    //--------------------------------------------------------------
    /// \brief			The plugin instance
    //--------------------------------------------------------------
    IHardwarePlugin * m_pPlugin;//TODO : rendre thread-safe

    //--------------------------------------------------------------
    /// \brief			get the plugin instance configuration (through database)
    //--------------------------------------------------------------
    std::string getPluginConfiguration() const;
};
