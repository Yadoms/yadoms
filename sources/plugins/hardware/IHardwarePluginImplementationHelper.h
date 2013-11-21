#pragma once

#include "IHardwarePlugin.h"
#include "HardwarePluginInformation.h"


// TODO : move this block in a specific plateform header
#ifdef WIN32
#define EXPORT_MAIN_FUNCTIONS __declspec(dllexport)
#else
#define EXPORT_MAIN_FUNCTIONS 
#endif


// Plugin implementation helper
// see CHardwarePluginInformation documentation to know what formats are accepted for each field
#define IMPLEMENT_HARDWARE_PLUGIN(pluginClassName,pluginName,version,release,author,url)  \
extern "C"                                                                                \
{                                                                                         \
   EXPORT_MAIN_FUNCTIONS plugins::IHardwarePlugin* construct()                            \
   {                                                                                      \
      return new plugins::pluginClassName();                                              \
   }                                                                                      \
   EXPORT_MAIN_FUNCTIONS void destruct(plugins::IHardwarePlugin* pluginToDelete)          \
   {                                                                                      \
      delete pluginToDelete;                                                              \
   }                                                                                      \
                                                                                          \
   static const plugins::CHardwarePluginInformation                                       \
      PluginInformations(pluginName,version,release,author,url);                          \
   EXPORT_MAIN_FUNCTIONS const plugins::CHardwarePluginInformation& getInformation()      \
   {                                                                                      \
      return PluginInformations;                                                          \
   }                                                                                      \
}   
