#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <shared/plugin/yadomsApi/Capacity.h>

//--------------------------------------------------------------
/// \brief	Fake temperature sensor
/// \note   Use to simulate a temperature sensor, with 2 variables values :
///         - temperature : start at 25°, and vary from +- 0.0 to 1.0° at each read
///         - battery level : start at 100%, decrease by 1% at each read, to 20%
//--------------------------------------------------------------
class CFakeTemperatureSensor//TODO faire en sorte qu'il remonte 2 températures
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CFakeTemperatureSensor(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeTemperatureSensor();

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   //--------------------------------------------------------------
   void read();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) temperature
   /// \return     Temperature in °C
   //--------------------------------------------------------------
   double getTemperature() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) battery level
   /// \return     Battery level (0 to 100%)
   //--------------------------------------------------------------
   int getBatteryLevel() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (const value here) signal strengh
   /// \return     Signal strengh (0 to 100)
   //--------------------------------------------------------------
   int getRssi() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the device capacities
   /// \return     The list of device capacities
   //--------------------------------------------------------------
   static const std::vector<shared::plugin::yadomsApi::CCapacity>& getCapacities();

private:
   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Temperature in °C
   //--------------------------------------------------------------
   double m_temperature;

   //--------------------------------------------------------------
   /// \brief	    Battery level (0 to 100%)
   //--------------------------------------------------------------
   int m_batteryLevel;

   //--------------------------------------------------------------
   /// \brief	    Signal strengh (0 to 100)
   //--------------------------------------------------------------
   const int m_rssi;

   //--------------------------------------------------------------
   /// \brief	    Random number generator, used to simulate temperature variations
   //--------------------------------------------------------------
   boost::random::mt19937 m_gen;
   boost::random::uniform_int_distribution<> m_dist;

};

