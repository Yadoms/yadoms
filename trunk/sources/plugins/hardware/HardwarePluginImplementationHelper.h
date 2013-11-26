#pragma once

#include "IHardwarePlugin.h"
#include "HardwarePluginInformation.h"
#include "HardwarePluginConfigurationSchemaParameter.h"
#include "IHardwarePluginConfigurationProvider.h"
#include "tools/Export.h" //platform specific export definitions


// Plugin implementation helper
// see CHardwarePluginInformation documentation to know what formats are accepted for each field
#define IMPLEMENT_HARDWARE_PLUGIN(pluginClassName,pluginName,version,release,author,url)  \
   EXPORT_LIBRARY_FUNCTION IHardwarePlugin* construct()                                   \
   {                                                                                      \
      return new pluginClassName();                                                       \
   }                                                                                      \
   EXPORT_LIBRARY_FUNCTION void destruct(IHardwarePlugin* pluginToDelete)                 \
   {                                                                                      \
      delete pluginToDelete;                                                              \
   }                                                                                      \
                                                                                          \
   static const CHardwarePluginInformation                                                \
      PluginInformations(pluginName,version,release,author,url);                          \
   EXPORT_LIBRARY_FUNCTION const CHardwarePluginInformation& getInformation()             \
   {                                                                                      \
      return PluginInformations;                                                          \
   }

// Easy configuration helper
//TODO
#define IMPLEMENT_HARDWARE_PLUGIN_EASYCONF()                                              \
   static const CHardwarePluginConfigurationSchema                                             \
      PluginEasyConfSchema;                                                               \
   EXPORT_LIBRARY_FUNCTION const CHardwarePluginConfigurationSchema& getEasyConfSchema()       \
   {                                                                                      \
      return PluginEasyConfSchema;                                                        \
   }

// Easy configuration plugin parameter add
//TODO#define IMPLEMENT_HARDWARE_PLUGIN_ADD_EASYCONF_PARAMETER(name,type,default,enumValuesList)

