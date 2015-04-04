#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IWUConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IWUConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer &data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the API Key from the configuration
   /// \return     The API Key String
   //--------------------------------------------------------------
   virtual std::string getAPIKey() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the Localisation from the configuration
   /// \return     The Localisation String
   //--------------------------------------------------------------
   virtual std::string getLocalisation() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the Country from the configuration
   /// \return     The Country String
   //--------------------------------------------------------------

   virtual std::string getCountryOrState() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the State from the configuration
   /// \return     The State String
   //--------------------------------------------------------------

   virtual std::string getState() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get options enabled from the configuration
   //--------------------------------------------------------------

   virtual bool IsWindEnabled (void) const = 0;
   virtual bool IsStandardInformationEnabled (void ) const = 0;
   virtual bool IsAstronomyEnabled (void ) const = 0;
   virtual bool IsForecast3DaysEnabled (void ) const = 0;
};