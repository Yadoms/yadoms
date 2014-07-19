#pragma once

#include "ILoad.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Fake temperature sensor
/// \note   Use to simulate a temperature sensor, with 2 variables values :
///         - temperature : start at 25�, and vary from +- 0.0 to 1.0� at each read
///         - battery level : start at 100%, decrease by 1% at each read, to 20%
//--------------------------------------------------------------
class CRaspberryPITemperatureSensor
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CRaspberryPITemperatureSensor(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRaspberryPITemperatureSensor();

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   /// \return      true if the temparature has been read with success, other cases false
   //--------------------------------------------------------------
   bool read();

    //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;
   
   //--------------------------------------------------------------
   /// \brief	            Declare the device and its associated keywords
   /// \param[in] context  YadomsApi context to which declare the device
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYadomsApi> context);

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
   
   //--------------------------------------------------------------
   /// \brief	    Returns read the temperature of the Raspberry CPU
   /// \return     Temperature in °C
   //--------------------------------------------------------------
   virtual double getValue();

   //--------------------------------------------------------------
   /// \brief	    Returns the Capacity String
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getCapacity() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the Keyword String
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getKeyword() const;
   
private:
   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Temperature in °C
   //--------------------------------------------------------------
   double m_temperature;
};

