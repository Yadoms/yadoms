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
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getAPIKey() const;
   virtual std::string getLocalisation();
   virtual std::string getCountryOrState();
   virtual std::string getState() const;
   virtual bool IsConditionsIndividualKeywordsEnabled (void ) const;
   virtual bool IsLiveConditionsEnabled (void ) const;
   virtual bool IsRainIndividualKeywordsEnabled (void ) const;
   virtual bool IsAstronomyEnabled (void ) const;
   virtual bool IsForecast10DaysEnabled (void ) const;
   // [END] IWUConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    The localisation entrered in the configuration of the module
   //--------------------------------------------------------------
   std::string m_Localisation;

   //--------------------------------------------------------------
   /// \brief	    The country or the american state code
   //--------------------------------------------------------------
   std::string m_CountryOrState;
};