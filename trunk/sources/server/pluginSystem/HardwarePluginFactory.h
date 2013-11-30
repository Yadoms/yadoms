//
// HardwarePluginInstance.h
//
// yadoms-plugin factory
//
#pragma once

#include "tools/DynamicLibrary.hpp" //cmake choose the right platform class
#include "plugins/hardware/IHardwarePlugin.h"
#include "plugins/hardware/HardwarePluginInformation.h"

//--------------------------------------------------------------
/// \brief	this class is used to load a plugin file library and construct instance
//--------------------------------------------------------------
class CHardwarePluginFactory : public CDynamicLibrary
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
    /// \brief	    Free library file
    //-------------------------------------------------------------
    void unload();

	//--------------------------------------------------------------
    /// \brief	    Construct a plugin instance (call the contruct library method)
    /// \return     a new plugin instance
    //-------------------------------------------------------------
    IHardwarePlugin * construct();

    //--------------------------------------------------------------
    /// \brief	    Destruct a plugin instance
    /// \param [in] item: the plugin to destruct
    //-------------------------------------------------------------
    void destruct(IHardwarePlugin* item);
	
    //--------------------------------------------------------------
    /// \brief	    Get information about this hardware plugin
    /// \return     information about this hardware plugin
    //-------------------------------------------------------------
    const CHardwarePluginInformation& getInformation() const;

private:
    //-------------------------------------------------------------
    /// \brief	    Function pointer to "construct" exported function
    //-------------------------------------------------------------
    boost::function<IHardwarePlugin* ()> m_construct;
	
    //-------------------------------------------------------------
    /// \brief	    Function pointer to "destruct" exported function
    //-------------------------------------------------------------
    boost::function<void (IHardwarePlugin*)> m_destruct;

    //--------------------------------------------------------------
    /// \brief	    Pointer to the plugin getInfo method
    //--------------------------------------------------------------
    boost::function<const CHardwarePluginInformation& ()> m_getInformation;
};

