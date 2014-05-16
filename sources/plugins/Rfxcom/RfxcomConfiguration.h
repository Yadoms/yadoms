#pragma once

#include <shared/plugin/configuration/Configuration.h>
#include <shared/plugin/ImplementationHelper.h>
#include "IRfxcomConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CRfxcomConfiguration : public shared::plugin::configuration::CConfiguration, public IRfxcomConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcomConfiguration();

   // IRfxcomConfiguration implementation
   virtual std::string getSerialPort() const;
   // [END] IRfxcomConfiguration implementation
};

