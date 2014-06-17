#pragma once

#include <shared/plugin/configuration/Configuration.h>


//--------------------------------------------------------------
/// \brief	Configuration of the xpl plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CRfxLanXplConfiguration : public shared::plugin::configuration::CConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRfxLanXplConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Determine if an xplhub should be ran at start
   //--------------------------------------------------------------
   bool getStartXplhub() const;

   //--------------------------------------------------------------
   /// \brief	    Get the xpl hub local ip to use
   //--------------------------------------------------------------
   std::string getHubLocalIp() const;
};

