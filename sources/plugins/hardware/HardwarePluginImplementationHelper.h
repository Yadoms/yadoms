#pragma once

#include "IHardwarePlugin.h"
#include "HardwarePluginInformation.hpp"
#include "../../shared/shared/HardwarePluginConfiguration/HardwarePluginConfiguration.hpp"//TODO : chemin pas bô !
#include "tools/Export.h" //platform specific export definitions


// Plugin implementation helper
// see IHardwarePluginInformation documentation to know what formats are accepted for each field
#define IMPLEMENT_HARDWARE_PLUGIN(pluginClassName,pluginName,version,release,author,url)  \
   EXPORT_LIBRARY_FUNCTION IHardwarePlugin* construct()                                   \
   {                                                                                      \
      return new pluginClassName();                                                       \
   }                                                                                      \
                                                                                          \
   static const CHardwarePluginInformation                                                \
      PluginInformations(pluginName,version,release,author,url);                          \
   EXPORT_LIBRARY_FUNCTION const IHardwarePluginInformation& getInformation()             \
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
