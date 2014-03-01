#pragma once

#include "IPlugin.h"
#include "information/Information.h"
#include "configuration/Configuration.h"
#include <shared/Export.h>
#include <shared/FileSystemExtension.h>
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------
/// \brief		                  Plugin implementation helper
/// \param pluginClassName       The name of the main plugin class (implementing shared::plugin::IPlugin)
/// \note                        This macro create and export needed functions for the plugin
//--------------------------------------------------------------
#define IMPLEMENT_PLUGIN(pluginClassName)                                                                   \
   IMPLEMENT_GET_MODULE_PATH                                                                                \
   EXPORT_LIBRARY_FUNCTION shared::plugin::IPlugin* construct()                                             \
   {                                                                                                        \
      return new pluginClassName();                                                                         \
   }                                                                                                        \
                                                                                                            \
   static boost::shared_ptr<const shared::plugin::information::CInformation> Informations;                  \
   EXPORT_LIBRARY_FUNCTION const shared::plugin::information::IInformation& getInformation()                \
   {                                                                                                        \
      if (!Informations)                                                                                    \
         Informations.reset(new shared::plugin::information::CInformation(getModulePath()));                \
      return *Informations;                                                                                 \
   }\



//////////////////////////////////////////////////////////////////////////
// Implement configuration
//////////////////////////////////////////////////////////////////////////
// Call this macro in the plugin configuration class declaration
#define DECLARE_CONFIGURATION(PluginConfigurationClassName)                               \
   public: static const PluginConfigurationClassName& getPluginSchema();

// Call this macro in the plugin configuration class definition file (usually .cpp file)
#define IMPLEMENT_CONFIGURATION(PluginConfigurationClassName)                             \
   const PluginConfigurationClassName& PluginConfigurationClassName::getPluginSchema()    \
   {                                                                                      \
      static boost::shared_ptr<PluginConfigurationClassName> Configuration;               \
      if (Configuration)                                                                  \
         return *Configuration;   /* Already initialized */                               \
      Configuration.reset(new PluginConfigurationClassName);                              \
      Configuration->buildSchema();                                                       \
      return *Configuration;                                                              \
   }                                                                                      \
   EXPORT_LIBRARY_FUNCTION const shared::plugin::configuration::ISchema& getConfigurationSchemaInterface() \
   {                                                                                      \
      return PluginConfigurationClassName::getPluginSchema();                             \
   }
