#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class ISmsDialerConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	      Destructor
   //--------------------------------------------------------------
   virtual ~ISmsDialerConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer &data) = 0;

   //--------------------------------------------------------------
   /// \brief	      Device used for Gammu library (serial port)
   /// \return       The serial port used to communicate with phone
   //--------------------------------------------------------------
   virtual std::string getGammuPort() const = 0;

   //--------------------------------------------------------------
   /// \brief	      Protocol used for Gammu library. Available values are defined in GSM_Connections (gsmstate.c)
   /// \return       The protocol used to communicate with phone
   //--------------------------------------------------------------
   virtual std::string getGammuProtocol() const = 0;

   //--------------------------------------------------------------
   /// \brief	      Check if phone has PIN code
   /// \return       true if a PIN Code is configured
   //--------------------------------------------------------------
   virtual bool hasPINCode() const = 0;

   //--------------------------------------------------------------
   /// \brief	      Phone PIN code
   /// \return       The configured PIN code
   //--------------------------------------------------------------
   virtual std::string getPhonePIN() const = 0;
};

