#pragma once

#include "../../plugins/hardware/IHardwarePluginConfigurationProvider.h"

//--------------------------------------------------------------
/// \class Hardware plugin configuration provider
//--------------------------------------------------------------
class HardwarePluginConfigurationProviderImplementation : public IHardwarePluginConfigurationProvider
{
public:
   virtual ~HardwarePluginConfigurationProviderImplementation() {}

   // IHardwarePluginConfigurationProvider implementation
   virtual std::map<std::string, CHardwarePluginConfigurationSchemaParameter> get() const;
   // [END] IHardwarePluginConfigurationProvider implementation
};
