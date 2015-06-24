#pragma once

#include <shared/plugin/ImplementationHelper.h>
#include "ISmsDialerConfiguration.h"


//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CSmsDialerConfiguration : public ISmsDialerConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSmsDialerConfiguration();

   // ISmsDialerConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getGammuPort() const;
   virtual std::string getGammuProtocol() const;
   virtual std::string getPhonePIN() const;
   virtual bool hasPINCode() const;
   // [END] ISmsDialerConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

