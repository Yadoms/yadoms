#pragma once

#include "Interfaces/IHardwarePlugin.h"
#include "Information/Information.h"
#include "Configuration/Configuration.h"
#include <shared/Export.h>


//////////////////////////////////////////////////////////////////////////
// Plugin implementation helper
//////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////
// Implement configuration
//////////////////////////////////////////////////////////////////////////
// TODO : rendre Configuration thread-safe
// TODO : revoir cette macro si nécessaire
#define IMPLEMENT_CONFIGURATION                                                           \
   void buidConfigurationSchema(CHardwarePluginConfiguration& configuration);             \
   const CHardwarePluginConfiguration& getConfigurationSchema()                           \
   {                                                                                      \
      static boost::optional<CHardwarePluginConfiguration> Configuration;                 \
                                                                                          \
      if (Configuration)                                                                  \
         return Configuration.get();   /* Already initialized */                          \
      CHardwarePluginConfiguration conf;                                                  \
      buidConfigurationSchema(conf);                                                      \
      Configuration = conf;                                                               \
      return Configuration.get();                                                         \
   }                                                                                      \
   EXPORT_LIBRARY_FUNCTION const IHardwarePluginConfigurationSchema& getConfigurationSchemaInterface() \
   {                                                                                      \
      return getConfigurationSchema();                                                    \
   }                                                                                      \
   void buidConfigurationSchema(CHardwarePluginConfiguration& configuration)



