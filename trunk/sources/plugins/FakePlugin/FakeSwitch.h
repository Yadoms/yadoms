#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <shared/plugin/yadomsApi/IYadomsApi.h>

namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	Fake switch
/// \note   Use to simulate a switch, with 2 variables values :
///         - onOffSwitch : toggle its state at each read
///         - dimmableSwitch : random value at each read
//--------------------------------------------------------------
class CFakeSwitch
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceName    The device name
   /// \param[in] isDimmable    ON/OFF switch if false, dimmable switch if true
   //--------------------------------------------------------------
   CFakeSwitch(const std::string& deviceName, bool isDimmable = false);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CFakeSwitch();

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
   /// \brief	    Returns the fake switch device name
   /// \return     The device name
   //--------------------------------------------------------------
   const std::string& getDeviceName() const;

   //--------------------------------------------------------------
   /// \brief	    Returns the fake switch model
   /// \return     The list of device capacities
   //--------------------------------------------------------------
   static const std::string& getModel();

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   const std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	    Switch type
   //--------------------------------------------------------------
   const bool m_isDimmable;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with the switch
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CDimmable> m_dimmableSwitch;
   boost::shared_ptr<yApi::historization::CSwitch> m_switch;

   //--------------------------------------------------------------
   /// \brief	    Random number generator, used to simulate increments
   //--------------------------------------------------------------
   boost::random::mt19937 m_gen;
   boost::random::uniform_int_distribution<> m_dist;

};

