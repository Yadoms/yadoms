#pragma once

#include <shared/xpl/XplMessage.h>

//--------------------------------------------------------------
/// \brief	The RFXCom protocol interface
//--------------------------------------------------------------
class ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ITransceiver() {}

   //--------------------------------------------------------------
   /// \brief	                     Reset the transceiver
   //--------------------------------------------------------------
   virtual void sendReset() = 0;

   //--------------------------------------------------------------
   /// \brief	                     Transmit a Xpl message
   /// \param [in] xplMessage       The Xpl message to transmit
   //--------------------------------------------------------------
   virtual void send(const shared::xpl::CXplMessage& xplMessage) = 0;
};



