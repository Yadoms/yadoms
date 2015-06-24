#pragma once

#include <shared/plugin/configuration/Configuration.h>
#include <shared/plugin/ImplementationHelper.h>
#include "ISmsDialerConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CSmsDialerConfiguration : public shared::plugin::configuration::CConfiguration, public ISmsDialerConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSmsDialerConfiguration();

   // ISmsDialerConfiguration implementation
   virtual std::string getGammuPort() const;
   virtual std::string getGammuProtocol() const;
   virtual std::string getPhonePIN() const;
   // [END] ISmsDialerConfiguration implementation
};

