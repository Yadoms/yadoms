#pragma once

#include "ITransceiver.h"
#include "IPort.h"
#include "rfxcomMessages/IRfxcomMessage.h"
#include "rfxcomMessages/RFXtrxDefinitions.h"
#include "ISequenceNumberProvider.h"

//--------------------------------------------------------------
/// \brief	This class implement the RFXCom protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CTransceiver(boost::shared_ptr<IPort> port);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   virtual void sendReset();
   virtual void send(const shared::xpl::CXplMessage& xplMessage);
   // [END] ITransceiver implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a command message to the transceiver
   /// \param [in] command          Command type
   //--------------------------------------------------------------
   void sendCommand(unsigned char command);

   //--------------------------------------------------------------
   /// \brief	                     Adapt a buffer to be loggable
   /// \param [in] ptr              Buffer pointer
   /// \param [in] sier             Buffer size
   //--------------------------------------------------------------
   std::string msgToString(const void* ptr, size_t size) const;

   //--------------------------------------------------------------
   /// \brief	                     Create the corresponding RFXCom message associated with a XPL message
   /// \param [in] xplMessage       The received XPL message
   /// \return                      RFXCom message
   /// \throw shared::exception::CInvalidParameter if no corresponding RFXCom message was found (XPL message not supported by RFXCom)
   //--------------------------------------------------------------
   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> createRfxcomMessage(const shared::xpl::CXplMessage& xplMessage) const;

private:
   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<IPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;

   //--------------------------------------------------------------
   /// \brief  The message structure (keep here for better performance)
   //--------------------------------------------------------------
   RBUF m_request;
};



