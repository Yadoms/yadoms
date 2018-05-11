#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class INitramConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~INitramConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		 Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer &data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the UPS
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the output shutdown delay
   /// \return     Output shutdown delay (minutes)
   //--------------------------------------------------------------
   virtual boost::posix_time::time_duration outputShutdownDelay() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the output restore delay
   /// \return     Output restore delay (minutes)
   //--------------------------------------------------------------
   virtual boost::posix_time::time_duration outputRestoreDelay() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the output restore delay
   /// \return     Output restore delay (minutes)
   //--------------------------------------------------------------
   virtual bool outputRestoreDelayActivated() const = 0;
};