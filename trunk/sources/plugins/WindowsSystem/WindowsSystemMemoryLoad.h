#pragma once

#include "ILoad.h"

//--------------------------------------------------------------
/// \brief	Memory Load of the Windows System
/// \note   return the memory load for Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemMemoryLoad : public ILoad
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
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) memory load
   /// \return     Temperature in �C
   //--------------------------------------------------------------
   virtual double getValue();

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

