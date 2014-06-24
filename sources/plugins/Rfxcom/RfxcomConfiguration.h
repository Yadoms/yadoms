#pragma once

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
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getSerialPort() const;
   // [END] IRfxcomConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

