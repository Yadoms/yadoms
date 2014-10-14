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
   /// \brief	    Get the power loss filter delay
   /// \return     Delay (seconds), 0 means no filter
   //--------------------------------------------------------------
   virtual unsigned int powerLossFilterDelay() const = 0;
};

