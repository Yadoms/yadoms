#pragma once

#include <shared/plugin/Configuration/Configuration.h>
#include <shared/plugin/ImplementationHelper.h>


// An example of an enum type of parameter
enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

class CFakePluginConfiguration : public CHardwarePluginConfiguration
{
   // Call this macro to declare that plugin has a configuration
   DECLARE_CONFIGURATION(CFakePluginConfiguration)

public:
   virtual ~CFakePluginConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Build the configuration schema
   /// \note       CHardwarePluginConfiguration implementation
   //--------------------------------------------------------------
   virtual void doBuildSchema();
};

