#pragma once

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "../shared/ILoad.h"
#include "pdh.h"
#include <string>
#include <vector>

//typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t> > wstring;

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
   /// \brief	    Make a sensor read (compute new values)
   /// \return      true if the CPU load has been read with success, other cases false
   //--------------------------------------------------------------
   bool read();

   //--------------------------------------------------------------
   /// \brief	    Returns the sensor device ID
   /// \return     Device ID
   //--------------------------------------------------------------
   virtual const std::string& getDeviceId() const;

   //--------------------------------------------------------------
   /// \brief	    Returns read (computed) CPU load
   /// \return     Temperature in �C
   //--------------------------------------------------------------
   virtual double getValue() const;

private:

   //std::string CWindowsSystemCPULoad::utf16ToUTF8( const wstring &s );

   //--------------------------------------------------------------
   /// \brief	    Device ID
   //--------------------------------------------------------------
   const std::string m_deviceId;

   //--------------------------------------------------------------
   /// \brief	    Memory Load in %
   //--------------------------------------------------------------
   double m_CPULoad;

   PDH_HQUERY cpuQuery;
   PDH_HCOUNTER cpuTotal;
};

