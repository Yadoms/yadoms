#pragma once

#include <shared/plugin/configuration/Configuration.h>
#include <shared/plugin/ImplementationHelper.h>


//--------------------------------------------------------------
/// \brief	An example of an enum type of parameter
//--------------------------------------------------------------
enum EEnumType
{
   kEnumValue1 = 7,
   kEnumValue2 = 12,
   kEnumValue3
};

//--------------------------------------------------------------
/// \brief	Configuration of the fake plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CFakePluginConfiguration : public shared::plugin::configuration::CConfiguration
{
   // Call this macro to declare that plugin has a configuration
   DECLARE_CONFIGURATION(CFakePluginConfiguration)

public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakePluginConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Just for test, not needed for real plugin
   //--------------------------------------------------------------
   void trace();

protected:
   //--------------------------------------------------------------
   /// \brief	    Build the configuration schema
   /// \note       CConfiguration implementation
   //--------------------------------------------------------------
   virtual void doBuildSchema();
};

