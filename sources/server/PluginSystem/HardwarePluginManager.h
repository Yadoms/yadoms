//
// HardwarePluginManager.h
//
// yadoms-plugin manager
//
#pragma once

#include "HardwarePluginFactory.h"
#include <list>

//--------------------------------------------------------------
/// \brief	this class is used to manage plugin. 
///         It search for plugins into directories and generate plugin factories
//--------------------------------------------------------------
class CHardwarePluginManager
{
public:
    //--------------------------------------------------------------
    /// \brief			Constructor
    //--------------------------------------------------------------
    CHardwarePluginManager();

    //--------------------------------------------------------------
    /// \brief			Destructor
    //--------------------------------------------------------------
    virtual ~CHardwarePluginManager();

    //--------------------------------------------------------------
    /// \brief			Build the factories list
    /// \param [in]		initialDir : the directory containing plugins
    //--------------------------------------------------------------
    void buildPluginFactoryList(const std::string & initialDir);

    //--------------------------------------------------------------
    /// \brief			Free the factories list (free memory)
    //--------------------------------------------------------------
    void freePluginFactoryList();

    //--------------------------------------------------------------
    /// \brief			Get a plugin factory
    /// \param [in]		pluginName : the plugin name to search
    /// \return    		The plugin factory (NULL in case of error)
    //--------------------------------------------------------------
    CHardwarePluginFactory * getFactory(const std::string & pluginName);

private:
    //--------------------------------------------------------------
    /// \brief			Returns all plugin libraries installed
    /// \param [in]		initialDir: initial search directory
    /// \return    		 a list of all found plugins (the plugin fullpath)
    //--------------------------------------------------------------
    std::list<std::string> findAvalaiblePlugins(const std::string & initialDir);

private:
    //--------------------------------------------------------------
    /// \brief			List of all found factories
    //--------------------------------------------------------------
    std::list<CHardwarePluginFactory *> m_pluginFactories;
};
