#pragma once

#include "ILoad.h"
#include "pdh.h"
#include <string>

//--------------------------------------------------------------
/// \brief	CPU Load of the Windows System
/// \note   return the CPU load under Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CWindowsSystemCPULoad(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemCPULoad();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) CPU load
   /// \return     CPU Load in %
   //--------------------------------------------------------------
   virtual double getValue();

private:

   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    CPU Load in %
   //--------------------------------------------------------------
   double m_CPULoad;

   //--------------------------------------------------------------
   /// \brief	    The Query for the CPU Load
   //--------------------------------------------------------------
   PDH_HQUERY cpuQuery;

   //--------------------------------------------------------------
   /// \brief	    Counter TOTAL CPU Load
   //--------------------------------------------------------------
   PDH_HCOUNTER cpuTotal;
};

