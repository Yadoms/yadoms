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
   virtual ~IWUConfiguration()
   {
   }

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the API Key from the configuration
   /// \return     The API Key String
   //--------------------------------------------------------------
   virtual std::string getAPIKey() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the Localisation from the configuration
   /// \return     The Localisation String
   //--------------------------------------------------------------
   virtual std::string getLocalisation() = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the Country from the configuration
   /// \return     The Country String
   //--------------------------------------------------------------
   virtual std::string getCountryOrState() = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the State from the configuration
   /// \return     The State String
   //--------------------------------------------------------------
   virtual std::string getState() const = 0;
};

