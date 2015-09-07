#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IMegatecUpsConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IMegatecUpsConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer &data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the UPS
   /// \return     The serial port name (system-dependent)
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Check if UPS beep is enable
   /// \return     true if UPS beep should be enable
   //--------------------------------------------------------------
   virtual bool upsBeepEnable() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the output shutdown delay
   /// \return     Output shutdown delay (minutes)
   //--------------------------------------------------------------
   virtual double outuputShutdownDelay() const = 0;

   //--------------------------------------------------------------
   /// \brief	    Get the output restore delay
   /// \return     Output restore delay (minutes)
   //--------------------------------------------------------------
   virtual double outuputRestoreDelay() const = 0;
};

