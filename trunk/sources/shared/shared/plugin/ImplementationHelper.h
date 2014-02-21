#pragma once

#include "Interfaces/IHardwarePlugin.h"
#include "information/Information.h"
#include "configuration/Configuration.h"
#include <shared/Export.h>


//////////////////////////////////////////////////////////////////////////
// Plugin implementation helper
//////////////////////////////////////////////////////////////////////////
// see shared::plugin::information::IInformation documentation to know what formats are accepted for each field
#define IMPLEMENT_HARDWARE_PLUGIN(pluginClassName,pluginName,version,release,author,url)  \
   EXPORT_LIBRARY_FUNCTION IHardwarePlugin* construct()                                   \
   {                                                                                      \
      return new pluginClassName();                                                       \
   }                                                                                      \
                                                                                          \
   static const shared::plugin::information::CInformation                                 \
      PluginInformations(pluginName,version,release,author,url);                          \
   EXPORT_LIBRARY_FUNCTION const shared::plugin::information::IInformation& getInformation() \
   {                                                                                      \
      return PluginInformations;                                                          \
   }


//////////////////////////////////////////////////////////////////////////
// Implement configuration
//////////////////////////////////////////////////////////////////////////
// Call this macro in the plugin configuration class declaration
#define DECLARE_CONFIGURATION(PluginConfigurationClassName)                               \
   public: static const PluginConfigurationClassName& getPluginSchema();

// Call this macro in the plugin configuration class definition file (usualy .cpp file)
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
   EXPORT_LIBRARY_FUNCTION const IHardwarePluginConfigurationSchema& getConfigurationSchemaInterface() \
   {                                                                                      \
      return PluginConfigurationClassName::getPluginSchema();                             \
   }
