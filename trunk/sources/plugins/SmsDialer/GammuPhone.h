#pragma once
#include <shared/event/EventHandler.hpp>
#include "ISmsDialerConfiguration.h"
#include "GammuPhoneConnection.h"   //TODO en faire une interface
#include "IPhone.h"
#include <gammu.h>

//--------------------------------------------------------------
/// \brief	Use GAMMU library (http://wammu.eu/) to dial with phone
//--------------------------------------------------------------
class CGammuPhone : protected shared::event::CEventHandler, public IPhone
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CGammuPhone(const ISmsDialerConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CGammuPhone();

   // IPhone implementation
   virtual void power(bool on);
   virtual void send(const std::string& targetPhoneNumer, const std::string& text);
  // [END] IPhone implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                  Gammu error handler
   /// \param[in] gsmError       Gammu error value
   /// \param[in] errorMessage   Error message to pass to exception if gsmError != ERR_NONE
   /// \throw  CPhoneException if error
   //--------------------------------------------------------------
   void handleGammuError(GSM_Error gsmError, const std::string& errorMessage) const;

   //--------------------------------------------------------------
   /// \brief	                  Callback called by Gammu when SMS was sent
   /// \param[in] gsmContext     The Gammu context
   /// \param[in] status         SMS sending status
   /// \param[in] MessageReference  Reference on SMS sent
   /// \param[in] user_data      Used here to store this
   //--------------------------------------------------------------
   static void sendSmsCallback(GSM_StateMachine *gsmContext, int status, int MessageReference, void * user_data);

private:
   //--------------------------------------------------------------
   /// \brief	   The plugin instance configuration
   //--------------------------------------------------------------
   const ISmsDialerConfiguration& m_configuration;

   //--------------------------------------------------------------
   /// \brief	   Phone connection
   //--------------------------------------------------------------
   CGammuPhoneConnection m_connection;
};



