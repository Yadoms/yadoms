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


#define ADD_CONFIGURATION_PARAMETER_BASE(type,name,description,defaultValue)                                      \
   {                                                                                                              \
      boost::shared_ptr<type> p(new type(name,description,defaultValue));                                         \
      configuration.AddParameter(p);                                                                              \
   }

#define ADD_CONFIGURATION_PARAMETER_INT(name,description,defaultValue)                                            \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationIntParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_DOUBLE(name,description,defaultValue)                                         \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationDoubleParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_STRING(name,description,defaultValue)                                         \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationStringParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_BOOL(name,description,defaultValue)                                           \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationBoolParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_ENUM(enumType,name,description,defaultValue,valueNames)                       \
   {                                                                                                              \
      boost::shared_ptr<CHardwarePluginConfigurationEnumParameter <enumType> >                                    \
      p(new CHardwarePluginConfigurationEnumParameter<enumType>(name,description,defaultValue,valueNames));       \
      configuration.AddParameter(p);                                                                              \
   }

#define ADD_CONFIGURATION_PARAMETER_SERIAL_PORT(name,description,defaultValue)                                    \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationSerialPortParameter,name,description,defaultValue)

#define ADD_CONFIGURATION_PARAMETER_BITS_FIELD(name,description,items)                                            \
   ADD_CONFIGURATION_PARAMETER_BASE(CHardwarePluginConfigurationBitsFieldParameter,name,description,items)

//TODO : ajouter un type list