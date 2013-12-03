//
// HardwarePluginInstance.h
//
// yadoms-plugin factory
//
#pragma once

#include "tools/DynamicLibrary.hpp" //cmake choose the right platform class
#include "plugins/hardware/IHardwarePlugin.h"
#include "plugins/hardware/HardwarePluginInformation.h"
#include "plugins/hardware/HardwarePluginConfiguration.hpp"

//--------------------------------------------------------------
/// \brief	this class is used to load a plugin file library and construct instance
//--------------------------------------------------------------
class CHardwarePluginFactory : public CDynamicLibrary
{
public:
    //--------------------------------------------------------------
    /// \brief	Constructor
    //--------------------------------------------------------------
    CHardwarePluginFactory(const std::string & initialDir);

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
    IHardwarePlugin* construct();
	
    //--------------------------------------------------------------
    /// \brief	    Get information about this hardware plugin
    /// \return     information about this hardware plugin
    //-------------------------------------------------------------
    const CHardwarePluginInformation& getInformation() const;

    //--------------------------------------------------------------
    /// \brief	    Get plugin default configuration
    /// \return     default configuration if configuration is available for this plugin
    //-------------------------------------------------------------
    const boost::optional<const CHardwarePluginConfiguration&> getDefaultConfiguration() const;

    //--------------------------------------------------------------
    /// \brief	    Format the plugin informations
    /// \return     Formated string containing plugin informations
    //-------------------------------------------------------------
    std::string formatPluginInformations() const;

private:
   //-------------------------------------------------------------
   /// \brief	    Plugin directory
   //-------------------------------------------------------------
   const std::string & m_initialDir;

    //-------------------------------------------------------------
    /// \brief	    Function pointer to "construct" exported function
    //-------------------------------------------------------------
    boost::function<IHardwarePlugin* ()> m_construct;

    //--------------------------------------------------------------
    /// \brief	    Pointer to the plugin getInformation method
    //--------------------------------------------------------------
    boost::function<const CHardwarePluginInformation& ()> m_getInformation;

    //--------------------------------------------------------------
    /// \brief	    Pointer to the plugin getConfiguration method (optional)
    //--------------------------------------------------------------
    boost::function<const CHardwarePluginConfiguration& ()> m_getDefaultConfiguration;
};

