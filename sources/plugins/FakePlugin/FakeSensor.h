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
   /// \param[in] deviceName    The device name
   //--------------------------------------------------------------
   CFakeSensor(const std::string& deviceName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeSensor();

   //--------------------------------------------------------------
   /// \brief	            Declare keywords associated with this device
   /// \param[in] context  YadomsApi context
   //--------------------------------------------------------------
   void declareKeywords(boost::shared_ptr<yApi::IYadomsApi> context);

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
   /// \brief	    Returns the fake sensor device name
   /// \return     The device name
   //--------------------------------------------------------------
   const std::string& getDeviceName() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor model
   /// \return     The list of device capacities
   //--------------------------------------------------------------
   static const std::string& getModel();

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with temperature1
   //--------------------------------------------------------------
   yApi::commands::CTemperature m_temperature1;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with temperature1
   //--------------------------------------------------------------
   yApi::commands::CTemperature m_temperature2;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with battery level
   //--------------------------------------------------------------
   yApi::commands::CBatteryLevel m_batteryLevel;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with signal strengh
   //--------------------------------------------------------------
   yApi::commands::CRssi m_rssi;

   //--------------------------------------------------------------
   /// \brief	    Random number generator, used to simulate temperature variations
   //--------------------------------------------------------------
   boost::random::mt19937 m_gen;
   boost::random::uniform_int_distribution<> m_dist;

};

