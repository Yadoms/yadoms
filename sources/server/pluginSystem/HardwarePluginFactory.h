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
#include "InvalidPluginException.hpp"

//--------------------------------------------------------------
/// \brief	this class is used to load a plugin file library and construct instance
//--------------------------------------------------------------
class CHardwarePluginFactory : public CDynamicLibrary
{
public:
   //--------------------------------------------------------------
   /// \brief	      Get plugin informations (without to load it)
   /// \param [in]   libraryPath: the plugin path
   /// \return       Plugin associated informations
   /// \throw        CInvalidPluginException if plugin is not valid
   //--------------------------------------------------------------
   static CHardwarePluginInformation getInformation(const boost::filesystem::path& libraryPath);

   //--------------------------------------------------------------
   /// \brief	    Get plugin default configuration
   /// \param [in]   libraryPath: the plugin path
   /// \return     default configuration if configuration is available for this plugin
   /// \throw        CInvalidPluginException if plugin is not valid
   //-------------------------------------------------------------
   static const boost::optional<const CHardwarePluginConfiguration&> getDefaultConfiguration(const boost::filesystem::path& libraryPath);

public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param [in] libraryPath: the plugin path
   /// \throw      CInvalidPluginException if plugin is not recognized
   //--------------------------------------------------------------
   CHardwarePluginFactory(const boost::filesystem::path& libraryPath);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginFactory();

   //--------------------------------------------------------------
   /// \brief	    Construct a plugin instance (call the contruct library method)
   /// \return     a new plugin instance
   //-------------------------------------------------------------
   IHardwarePlugin* construct() const;

   //--------------------------------------------------------------
   /// \brief	    Get the plugin library path
   /// \return     the plugin library path
   //-------------------------------------------------------------
   const boost::filesystem::path& getLibraryPath() const;

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

private:
   //--------------------------------------------------------------
   /// \brief	    Load a plugin file
   /// \throw      CInvalidPluginException if plugin is not recognized
   //-------------------------------------------------------------
   void load();

   //--------------------------------------------------------------
   /// \brief	    Free library file
   //-------------------------------------------------------------
   void unload();

   //-------------------------------------------------------------
   /// \brief	    Plugin library path
   //-------------------------------------------------------------
   const boost::filesystem::path m_libraryPath;

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
