#include "stdafx.h"
#include "HardwarePluginConfigurationProviderImplementation.h"

//TODO La gestion de la conf n'ait pas encore satisfaisante, à suivre donc...
std::map<std::string, CHardwarePluginConfigurationSchemaParameter> HardwarePluginConfigurationProviderImplementation::get() const
{
   // TODO : récupérer la conf de la base
   std::map<std::string, CHardwarePluginConfigurationSchemaParameter> pluginParameters;
   CHardwarePluginConfigurationSchemaParameter serialPortParameter = {"Serial port"   , CHardwarePluginConfigurationSchemaParameter::kString   , "tty3"       , ""};
   pluginParameters["Serial port"] = serialPortParameter;
   CHardwarePluginConfigurationSchemaParameter boolParameter = {"BoolParameter"   , CHardwarePluginConfigurationSchemaParameter::kBool   , "true"       , ""};
   pluginParameters["BoolParameter"] = boolParameter;
   CHardwarePluginConfigurationSchemaParameter enumParameter = {"EnumParameter"   , CHardwarePluginConfigurationSchemaParameter::kEnum   , "EnumValue2"       , ""};
   pluginParameters["EnumParameter"] = enumParameter;

   return pluginParameters;
}
