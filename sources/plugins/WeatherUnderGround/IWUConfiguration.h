#pragma once
#include <shared/DataContainer.h>
#include <shared/ILocation.h>

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
   {}

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
   /// \brief	    Return if the plugin Location is enabled
   /// \return     true if the plugin Location is enabled
   //--------------------------------------------------------------
   virtual bool pluginLocationEnabled() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the Location from the configuration
   /// \return     The location entered in the plugin
   //--------------------------------------------------------------
   virtual boost::shared_ptr<const shared::ILocation> getLocation() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get options enabled from the configuration
   //--------------------------------------------------------------
   virtual bool isLiveConditionsEnabled(void) const = 0;
   virtual bool isConditionsIndividualKeywordsEnabled(void) const = 0;
   virtual bool isRainIndividualKeywordsEnabled(void) const = 0;
   virtual bool isTempIndividualKeywordsEnabled(void) const = 0;
   virtual bool isAstronomyEnabled(void) const = 0;
   virtual bool isForecast10DaysEnabled(void) const = 0;
};