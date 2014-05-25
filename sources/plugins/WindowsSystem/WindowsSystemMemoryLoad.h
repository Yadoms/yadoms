#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

//--------------------------------------------------------------
/// \brief	Memory Load of the Windows System
/// \note   return the memory load under Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemMemoryLoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CWindowsSystemMemoryLoad(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemMemoryLoad();

   //--------------------------------------------------------------
   /// \brief	    Make a sensor read (compute new values)
   /// \return      true if the memory load has been read with success, other cases false
      //--------------------------------------------------------------
   bool read();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) memory load
   /// \return     Temperature in �C
   //--------------------------------------------------------------
   double getMemoryLoad() const;

private:
   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Memory Load in %
   //--------------------------------------------------------------
   double m_memoryLoad;
};

