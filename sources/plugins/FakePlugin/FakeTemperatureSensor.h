#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <shared/plugin/yadomsApi/Capacity.h>

namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Fake temperature sensor
/// \note   Use to simulate a temperature sensor, with 2 variables values :
///         - temperature : start at 25°, and vary from +- 0.0 to 1.0° at each read
///         - battery level : start at 100%, decrease by 1% at each read, to 20%
//--------------------------------------------------------------
class CFakeTemperatureSensor
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
   /// \brief	    Returns read (computed) temperature 1
   /// \return     Temperature in °C
   //--------------------------------------------------------------
   double getTemperature1() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) temperature 2
   /// \return     Temperature in °C
   //--------------------------------------------------------------
   double getTemperature2() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) battery level
   /// \return     Battery level (0 to 100%)
   //--------------------------------------------------------------
   int getBatteryLevel() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (const value here) signal strength
   /// \return     Signal strength (0 to 100)
   //--------------------------------------------------------------
   int getRssi() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the device capacities
   /// \return     The list of device capacities
   //--------------------------------------------------------------
   static const std::vector<yApi::CCapacity>& getCapacities();

private:
   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Temperatures in °C
   //--------------------------------------------------------------
   double m_temperature1;
   double m_temperature2;

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

