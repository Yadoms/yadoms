#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Fake temperature sensor
/// \note   Use to simulate a temperature sensor, with some variables values :
///         - 2 temperatures : start at 25°, and vary from +- 0.0 to 1.0° at each read
///         - battery level : start at 100%, decrease by 1% at each read, to 20%
///         - current : start at 2A, and vary from +- 0.0 to 5.0A at each read
///         - signal power : fixed at 50%
//--------------------------------------------------------------
class CFakeSensor
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   //--------------------------------------------------------------
   explicit CFakeSensor(std::string deviceName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeSensor() = default;

   //--------------------------------------------------------------
   /// \brief	            Declare device and associated keywords if necessary
   /// \param[in] api      yPluginApi context
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   //--------------------------------------------------------------
   void read() const;

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] api      yPluginApi context to which historize data
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor device name
   /// \return     The device name
   //--------------------------------------------------------------
   const std::string& getDeviceName() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor model
   /// \return     The fake sensor model
   //--------------------------------------------------------------
   static const std::string& getModel();

   //--------------------------------------------------------------
   /// \brief	    Returns the fake sensor type
   /// \return     The fake sensor type
   //--------------------------------------------------------------
   static const std::string& getType();

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with temperature1
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature1;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with temperature1
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature2;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with battery level
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with current consumption
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CCurrent> m_current;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with signal strength
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSignalPower> m_signalPower;

   //--------------------------------------------------------------
   /// \brief	Historizers vector
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_historizers;
};
