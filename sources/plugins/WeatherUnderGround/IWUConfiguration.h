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
   /// \brief	    Get the Localisation from the configuration
   /// \return     The Localisation String
   //--------------------------------------------------------------

   virtual bool IsTemperatureEnabled (void) const = 0;
   virtual bool IsDewPointEnabled (void) const = 0;
   virtual bool IsPressureEnabled (void) const = 0;
   virtual bool IsHumidityEnabled (void) const = 0;
   virtual bool IsVisibilityEnabled (void) const = 0;
   virtual bool IsUVEnabled (void) const = 0;
   virtual bool IsRain1hrEnabled (void) const = 0;
   virtual bool IsRainDayEnabled (void) const = 0;
   virtual bool IsWindEnabled (void) const = 0;
   virtual bool IsFeelsLikeEnabled (void) const = 0;
   virtual bool IsWinChillEnabled (void) const = 0;
   virtual bool IsWeatherConditionIconEnabled (void ) const = 0;
};

