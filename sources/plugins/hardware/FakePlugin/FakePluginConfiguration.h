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


   // If parameters are needed for the plugin, just use the IMPLEMENT_CONFIGURATION macro
   // to create the configuration schema and add parameters
   //TODO remettre la macro IMPLEMENT_CONFIGURATION
public:
   static const CFakePluginConfiguration& getSchema()
   {
      static boost::shared_ptr<CFakePluginConfiguration> Configuration;

      if (Configuration)
         return *Configuration;   /* Already initialized */
      Configuration.reset(new CFakePluginConfiguration);
      Configuration->buildSchema();
      return *Configuration;
   }
};

