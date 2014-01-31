#pragma once

#include <shared/HardwarePlugin/Configuration/Configuration.h>


// An example of an enum type of parameter
enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

class CFakePluginConfiguration : public CHardwarePluginConfiguration
{
public:
   virtual ~CFakePluginConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Build the configuration schema
   /// \note       CHardwarePluginConfiguration implementation
   //--------------------------------------------------------------
   virtual void doBuildSchema();
};

