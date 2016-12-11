#pragma once

#include "IWUConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CWUConfiguration : public IWUConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CWUConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWUConfiguration();

   // IWUConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   std::string getAPIKey() const override;
   std::string getLocalisation() override;
   std::string getCountryOrState() override;
   std::string getState() const override;
   // [END] IWUConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    The localisation entrered in the configuration of the module
   //--------------------------------------------------------------
   std::string m_localisation;

   //--------------------------------------------------------------
   /// \brief	    The country or the american state code
   //--------------------------------------------------------------
   std::string m_countryOrState;
};

