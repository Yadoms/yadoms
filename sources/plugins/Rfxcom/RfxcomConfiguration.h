#pragma once

#include <shared/DataContainer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "IRfxcomConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CRfxcomConfiguration : public shared::CDataContainer, public IRfxcomConfiguration
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

