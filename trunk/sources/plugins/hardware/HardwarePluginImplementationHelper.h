#pragma once

#include "IHardwarePlugin.h"
#include "HardwarePluginInformation.h"
#include "HardwarePluginConfiguration.hpp"
#include "tools/Export.h" //platform specific export definitions


// Plugin implementation helper
// see CHardwarePluginInformation documentation to know what formats are accepted for each field
#define IMPLEMENT_HARDWARE_PLUGIN(pluginClassName,pluginName,version,release,author,url)  \
   EXPORT_LIBRARY_FUNCTION IHardwarePlugin* construct()                                   \
   {                                                                                      \
      return new pluginClassName();                                                       \
   }                                                                                      \
                                                                                          \
   static const CHardwarePluginInformation                                                \
      PluginInformations(pluginName,version,release,author,url);                          \
   EXPORT_LIBRARY_FUNCTION const CHardwarePluginInformation& getInformation()             \
   {                                                                                      \
      return PluginInformations;                                                          \
   }

// Implement configuration
#define IMPLEMENT_CONFIGURATION                                                           \
   static boost::optional<CHardwarePluginConfiguration> Configuration;                    \
   void buidDefaultConfiguration(CHardwarePluginConfiguration& configuration);            \
   EXPORT_LIBRARY_FUNCTION const CHardwarePluginConfiguration& getDefaultConfiguration()  \
   {                                                                                      \
      if (Configuration)                                                                  \
         return Configuration.get();   /* Already initialized */                          \
      CHardwarePluginConfiguration conf;                                                  \
      buidDefaultConfiguration(conf);                                                     \
      Configuration = conf;                                                               \
      return Configuration.get();                                                         \
   }                                                                                      \
   void buidDefaultConfiguration(CHardwarePluginConfiguration& configuration)
//TODO : voir si on ne peut pas y ajouter l'impl�mentation de IHardwarePlugin::updateConfiguration
