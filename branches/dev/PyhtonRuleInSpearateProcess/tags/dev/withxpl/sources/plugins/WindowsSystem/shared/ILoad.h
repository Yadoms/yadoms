#pragma once

//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/uniform_int_distribution.hpp>

//--------------------------------------------------------------
/// \brief	Interface Load definition
/// \note   return the load in poucentage for all system information
//--------------------------------------------------------------
class ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ILoad() {};

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) memory load
   /// \return     Temperature in ?C
   //--------------------------------------------------------------
   virtual double getValue() const = 0;
};

