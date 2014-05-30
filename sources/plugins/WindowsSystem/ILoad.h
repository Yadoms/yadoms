#pragma once

//--------------------------------------------------------------
/// \brief	Interface Load definition
/// \note   return the load in pourcentage of a value
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
   /// \brief	    Returns read (computed) load value
   /// \return     value in pourcentage
   //--------------------------------------------------------------
   virtual double getValue() = 0;
};

