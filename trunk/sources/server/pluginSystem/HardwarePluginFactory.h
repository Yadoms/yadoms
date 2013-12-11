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
   /// \brief	      Get plugin informations (without to load it)
   /// \param [in]   libraryPath: the plugin path
   /// \return       Plugin associated informations
   /// \throw        CInvalidPluginException if plugin is not valid
   //--------------------------------------------------------------
   static CHardwarePluginInformation getInformation(const boost::filesystem::path& libraryPath);

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
   /// \brief	    Load a plugin file
   /// \param [in] libraryPath: the plugin path
   /// \throw      CInvalidPluginException if plugin is not recognized
   //-------------------------------------------------------------
   void load(const boost::filesystem::path& libraryPath);

   //--------------------------------------------------------------
   /// \brief	    Free library file
   //-------------------------------------------------------------
   void unload();

   //--------------------------------------------------------------
   /// \brief	    Construct a plugin instance (call the contruct library method)
   /// \return     a new plugin instance
   //-------------------------------------------------------------
   IHardwarePlugin* construct() const;

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

//--------------------------------------------------------------
/// \class Configuration loading error exception
//--------------------------------------------------------------
class CInvalidPluginException : public std::exception
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  optionsDecription   Full options description structure
   /// \param[in]  message             Extended message (usefull if error in the command line)
   //--------------------------------------------------------------
   CInvalidPluginException(const std::string& plugin) : m_plugin(plugin) {}

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CInvalidPluginException() throw() {}

   //--------------------------------------------------------------
   /// \brief	    Build full message explaining error reason and command line usage
   /// \return     message explaining error reason and command line usage
   //--------------------------------------------------------------
   virtual const char* what() const throw() { return std::string("Invalid plugin found " + m_plugin).c_str(); }

protected:
   std::string m_plugin;
};