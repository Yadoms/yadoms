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
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWUConfiguration();

   // IWUConfiguration implementation
   virtual void initializeWith(const shared::CDataContainer &data);
   virtual std::string getAPIKey() const;
   virtual std::string getLocalisation() const;
   bool IsTemperatureEnabled (void) const;
   bool IsDewPointEnabled (void) const;
   bool IsPressureEnabled (void) const;
   bool IsHumidityEnabled (void) const;
   bool IsVisibilityEnabled (void) const;
   bool IsUVEnabled (void) const;
   bool IsRain1hrEnabled (void) const;
   bool IsRainDayEnabled (void) const;
   bool IsWindEnabled (void) const;
   bool IsFeelsLikeEnabled (void) const;
   bool IsWinChillEnabled (void) const;
   bool IsWeatherConditionIconEnabled (void ) const;
   // [END] IWUConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;

   //--------------------------------------------------------------
   /// \brief	    State of each options
   //--------------------------------------------------------------

   bool m_isTemperatureEnabled;
   bool m_isDewPointEnabled;
   bool m_isPressureEnabled;
   bool m_isHumidityEnabled;
   bool m_isVisibilityEnabled;
   bool m_isUVEnabled;
   bool m_isRain1hrEnabled;
   bool m_isRainDayEnabled;
   bool m_isWindEnabled;
   bool m_isFeelsLikeEnabled;
   bool m_isWinChillEnabled;
   bool m_isWeatherConditionIconEnabled;
};