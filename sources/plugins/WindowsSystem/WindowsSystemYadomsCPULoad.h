#pragma once

#include "ILoad.h"

//--------------------------------------------------------------
/// \brief	CPU Load of the Yadoms Process
/// \note   return the CPU load of the Yadoms Process under Windows Operating System
//--------------------------------------------------------------
class CWindowsSystemYadomsCPULoad : public ILoad
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] deviceId    The device ID
   //--------------------------------------------------------------
   CWindowsSystemYadomsCPULoad(const std::string & deviceId);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWindowsSystemYadomsCPULoad();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) CPU load of the Yadoms Process
   /// \return     CPU Load in %
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
   double m_CPULoad;

   //--------------------------------------------------------------
   /// \brief	    Number of total ticks, System ticks, User Ticks
   //--------------------------------------------------------------
   unsigned __int64 lastCPU, lastSysCPU, lastUserCPU;

   //--------------------------------------------------------------
   /// \brief	    Number of processors
   //--------------------------------------------------------------
   int numProcessors;

   //--------------------------------------------------------------
   /// \brief	    Handle of Yadoms
   //--------------------------------------------------------------
   HANDLE self;
};

