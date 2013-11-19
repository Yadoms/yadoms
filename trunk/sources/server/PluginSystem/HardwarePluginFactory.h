//
// HardwarePluginInstance.h
//
// yadoms-plugin factory
//
#pragma once

#include <string>
#include <boost/function.hpp>
#include "PluginFactory.hpp" //cmake choose the right platform class
#include "../plugins/hardware/IHardwarePlugin.h"
#include "../plugins/hardware/HardwarePluginInformation.h"

//--------------------------------------------------------------
/// \brief	this class is used to load a plugin file library and construct instance
//--------------------------------------------------------------
class CHardwarePluginFactory : public CPluginFactory<plugins::IHardwarePlugin>
{
public:
    //--------------------------------------------------------------
    /// \brief	Constructor
    //--------------------------------------------------------------
    CHardwarePluginFactory();

    //--------------------------------------------------------------
    /// \brief	Destructor
    //--------------------------------------------------------------
    virtual ~CHardwarePluginFactory();

    //--------------------------------------------------------------
    /// \brief	    Load a plugin file
    /// \param [in] libraryFile: the plugin path
    /// \return     true is the plugin is loaded with success, else false
    //-------------------------------------------------------------
    bool load(const std::string & libraryFile);

    //--------------------------------------------------------------
    /// \brief	    Get information about this hardware plugin
    /// \return     information about this hardware plugin
    //-------------------------------------------------------------
    plugins::CHardwarePluginInformation * getInfo();

private:
    //--------------------------------------------------------------
    /// \brief	    Pointer to the plugin getInfo method
    //--------------------------------------------------------------
    boost::function< plugins::CHardwarePluginInformation* () > m_getInfo;
};

