#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <shared/plugin/yadomsApi/IYadomsApi.h>

namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Fake temperature sensor
/// \note   Use to simulate a temperature sensor, with 2 variables values :
///         - temperature : start at 25°, and vary from +- 0.0 to 1.0° at each read
///         - battery level : start at 100%, decrease by 1% at each read, to 20%
//--------------------------------------------------------------
class CFakeSensor
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CFakeSensor(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeSensor();

   //--------------------------------------------------------------
   /// \brief	            Declare the device and its associated keywords
   /// \param[in] context  YadomsApi context to which declare the device
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   //--------------------------------------------------------------
   void read();

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] context  YadomsApi context to which historize data
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor model
   /// \return     The list of device capacities
   //--------------------------------------------------------------
   static const std::string& getModel();

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

