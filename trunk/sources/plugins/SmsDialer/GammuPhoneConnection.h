#pragma once
#include "ISmsDialerConfiguration.h"
#include <gammu.h>

//--------------------------------------------------------------
/// \brief	Manage the phone connection
//--------------------------------------------------------------
class CGammuPhoneConnection
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor, establish the connection
   /// \throw  CPhoneException is unable to connect to phone
   //--------------------------------------------------------------
   CGammuPhoneConnection(const ISmsDialerConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	Destructor, close the connection
   //--------------------------------------------------------------
   virtual ~CGammuPhoneConnection();

   //--------------------------------------------------------------
   /// \brief	   Get the GSM context, used for each operation on connected phone
   /// \return    The current Gsm context (NULL is not connected)
   //--------------------------------------------------------------
   GSM_StateMachine* getGsmContext();

protected:
   //--------------------------------------------------------------
   /// \brief	Phone connection
   /// \throw  CPhoneException if trouble connecting to phone
   //--------------------------------------------------------------
   void connect();

   //--------------------------------------------------------------
   /// \brief	Phone disconnection
   /// \throw  CPhoneException if trouble disconnecting from phone
   //--------------------------------------------------------------
   void disconnect();

   //--------------------------------------------------------------
   /// \brief	                  Gammu error handler
   /// \param[in] gsmError       Gammu error value
   /// \param[in] errorMessage   Error message to pass to exception if gsmError != ERR_NONE
   /// \throw  CPhoneException if error
   //--------------------------------------------------------------
   void handleGammuError(GSM_Error gsmError, const std::string& errorMessage) const;

private:
   //--------------------------------------------------------------
   /// \brief	   The plugin instance configuration
   //--------------------------------------------------------------
   const ISmsDialerConfiguration& m_configuration;

   //--------------------------------------------------------------
   /// \brief	   The GSM context, used for each operation on phone
   //--------------------------------------------------------------
   GSM_StateMachine* m_gsmContext;
};



