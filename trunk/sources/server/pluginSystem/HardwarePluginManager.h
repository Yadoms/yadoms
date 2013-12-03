//
// HardwarePluginManager.h
//
// yadoms-plugin manager
//
#pragma once

#include "HardwarePluginFactory.h"

//--------------------------------------------------------------
/// \brief	this class is used to manage plugin. 
///         It search for plugins into directories and generate plugin factories
//--------------------------------------------------------------
class CHardwarePluginManager
{
public:
   //--------------------------------------------------------------
   /// \brief			Constructor
   /// \param [in]   initialDir: initial plugins search directory
   //--------------------------------------------------------------
   CHardwarePluginManager(const std::string & initialDir);

   //--------------------------------------------------------------
   /// \brief			Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginManager();

   //--------------------------------------------------------------
   /// \brief			Get a plugin factory
   /// \param [in]   pluginName : the plugin name to search
   /// \return       The plugin factory
   /// \throw        std::out_of_range if plugin doesn't exist
   //--------------------------------------------------------------
   const boost::shared_ptr<CHardwarePluginFactory> getFactory(const std::string & pluginName);

private:
   //--------------------------------------------------------------
   /// \brief			Returns all plugin libraries installed
   /// \param [in]		initialDir: initial search directory
   /// \return    		 a list of all found plugins (the plugin fullpath)
   //--------------------------------------------------------------
   std::list<std::string> findAvalaiblePlugins(const std::string & initialDir);

   //--------------------------------------------------------------
   /// \brief			Build the factories list and load plugins
   /// \param [in]		initialDir : the directory containing plugins
   //--------------------------------------------------------------
   void buildPluginFactoryList(const std::string & initialDir);

private:
   //--------------------------------------------------------------
   /// \brief			Map of all found factories (key are plugin file names)
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<CHardwarePluginFactory> > m_pluginFactories;
};
