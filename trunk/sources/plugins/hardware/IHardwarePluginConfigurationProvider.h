#pragma once

#include <map>
#include "HardwarePluginConfigurationSchemaParameter.h"

//--------------------------------------------------------------
/// \class Hardware plugin configuration provider interface
//--------------------------------------------------------------
class IHardwarePluginConfigurationProvider
{
public:
   virtual ~IHardwarePluginConfigurationProvider() {}

   //--------------------------------------------------------------
   /// \brief     Get the Plugin instance configuration
   /// \return    Map of plugin parameters
   //--------------------------------------------------------------
   virtual std::map<std::string, CHardwarePluginConfigurationSchemaParameter> get() const = 0;
};

