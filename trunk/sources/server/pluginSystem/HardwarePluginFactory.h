//
// HardwarePluginInstance.h
//
// yadoms-plugin factory
//
#pragma once

#include <shared/DynamicLibrary.hpp>
#include <shared/plugin/IPlugin.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/plugin/configuration/ISchema.h>
#include "InvalidPluginException.hpp"

//--------------------------------------------------------------
/// \brief	this class is used to load a plugin file library and construct instance
//--------------------------------------------------------------
class CHardwarePluginFactory : public shared::CDynamicLibrary
{
public:
   //--------------------------------------------------------------
   /// \brief	      Get plugin informations (without to load it)
   /// \param [in]   libraryPath: the plugin path
   /// \return       Plugin associated informations
   /// \throw        CInvalidPluginException if plugin is not valid
   //--------------------------------------------------------------
   static boost::shared_ptr<const shared::plugin::information::IInformation> getInformation(const boost::filesystem::path& libraryPath);

   //--------------------------------------------------------------
   /// \brief	      Get plugin configuration schema
   /// \param [in]   libraryPath: the plugin path
   /// \return       Configuration schema if available for this plugin (empty string if not)
   /// \throw        CInvalidPluginException if plugin is not valid
   //-------------------------------------------------------------
   static std::string getConfigurationSchema(const boost::filesystem::path& libraryPath);

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
   /// \brief	    Construct a plugin instance (call the construct library method)
   /// \return     a new plugin instance
   //-------------------------------------------------------------
   shared::plugin::IPlugin* construct() const;

   //--------------------------------------------------------------
   /// \brief	    Get the plugin library path
   /// \return     the plugin library path
   //-------------------------------------------------------------
   const boost::filesystem::path& getLibraryPath() const;

   //--------------------------------------------------------------
   /// \brief	    Get information about this plugin
   /// \return     information about this plugin
   //-------------------------------------------------------------
   boost::shared_ptr<const shared::plugin::information::IInformation> getInformation() const;

   //--------------------------------------------------------------
   /// \brief	    Get plugin configuration schema
   /// \return     Configuration schema if available for this plugin (empty string if not)
   //-------------------------------------------------------------
   std::string getConfigurationSchema() const;
    
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
   boost::function<shared::plugin::IPlugin* ()> m_construct;

   //--------------------------------------------------------------
   /// \brief	    Pointer to the plugin getInformation method
   //--------------------------------------------------------------
   boost::function<const shared::plugin::information::IInformation& ()> m_getInformation;

   //--------------------------------------------------------------
   /// \brief	    Pointer to the plugin getConfigurationSchemaInterface method (optional)
   //--------------------------------------------------------------
   boost::function<const shared::plugin::configuration::ISchema& ()> m_getConfigurationSchema;
};
