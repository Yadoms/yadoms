#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Fake sensor, with no history 
/// \note   Can be useful to manage forecast values, for example from weather forecast provider.
///         - temperature : start at 25°, and vary from +- 0.0 to 1.0° at each read
///         - humidity : start at 80%, and vary from +- 0.0 to 1.0% at each read
//--------------------------------------------------------------
class CFakeNoHistorySensor
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   //--------------------------------------------------------------
   explicit CFakeNoHistorySensor(const std::string& deviceName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeNoHistorySensor() = default;

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
   /// \brief	The keyword associated with temperature
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with humidity
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CHumidity> m_humidity;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with en incremental counter
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CCounter> m_incrementalCounter;

   //--------------------------------------------------------------
   /// \brief	Historizers vector
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>> m_historizers;
};

