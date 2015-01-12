#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Fake counter
/// \note   Use to simulate a counter, with 2 variables values :
///         - incrementCount : return from 0 to 15 at each read
///         - totalCount : increment from 0 to 15 at each read
//--------------------------------------------------------------
class CFakeCounter
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   //--------------------------------------------------------------
   CFakeCounter(const std::string& deviceName);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeCounter();

   //--------------------------------------------------------------
   /// \brief	            Declare device and associated keywords if necessary
   /// \param[in] context  yPluginApi context
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> context);

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   //--------------------------------------------------------------
   void read();

   //--------------------------------------------------------------
   /// \brief	            Send all sensor data to Yadoms
   /// \param[in] context  yPluginApi context to which historize data
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;

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
   /// \brief	The keyword associated with incremental counter
   //--------------------------------------------------------------
   yApi::historization::CCounter m_incrementCount;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with total counter
   //--------------------------------------------------------------
   yApi::historization::CCounter m_totalCount;

   //--------------------------------------------------------------
   /// \brief	    Random number generator, used to simulate increments
   //--------------------------------------------------------------
   boost::random::mt19937 m_gen;
   boost::random::uniform_int_distribution<> m_dist;

};

