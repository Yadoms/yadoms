#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IMegatecUpsConfiguration
{
public:
   virtual ~IMegatecUpsConfiguration() = default;

   //--------------------------------------------------------------
   /// \brief		 Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the UPS
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Check if UPS beep is enable
   /// \return     true if UPS beep enable
   //--------------------------------------------------------------
   virtual bool upsBeepEnable() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Check if autotest is enable
   /// \return     true if autotest enable
   //--------------------------------------------------------------
   virtual bool autotestEnable() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Autotest start time
   /// \return     Duration (only hours and minutes)
   //--------------------------------------------------------------
   virtual boost::posix_time::time_duration autotestStartTime() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the output shutdown delay
   /// \return     Output shutdown delay (minutes)
   //--------------------------------------------------------------
   virtual double outputShutdownDelay() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the output restore delay
   /// \return     Output restore delay (minutes)
   //--------------------------------------------------------------
   virtual double outputRestoreDelay() const = 0;
};
