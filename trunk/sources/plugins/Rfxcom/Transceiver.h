#pragma once

#include "ITransceiver.h"
#include "IPort.h"
#include "rfxcomMessages/IRfxcomMessage.h"
#include "rfxcomMessages/RFXtrxDefinitions.h"
#include "ISequenceNumberProvider.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

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
   virtual void send(boost::shared_ptr<yApi::IDeviceCommand> command);
   // [END] ITransceiver implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a command message to the transceiver
   /// \param [in] command          Command type
   //--------------------------------------------------------------
   void sendCommand(unsigned char command);

   //--------------------------------------------------------------
   /// \brief	                     Wait for the status answer from RFXCom
   /// \return                      true if status received, false if error
   //--------------------------------------------------------------
   bool waitStatus();

   //--------------------------------------------------------------
   /// \brief	                     Adapt a buffer to be loggable
   /// \param [in] ptr              Buffer pointer
   /// \param [in] size             Buffer size
   //--------------------------------------------------------------
   std::string msgToString(const void* ptr, size_t size) const;

   //--------------------------------------------------------------
   /// \brief	                     Create the corresponding RFXCom message associated with the command received from Yadoms
   /// \param [in] command          The received command
   /// \return                      RFXCom message
   /// \throw shared::exception::CInvalidParameter if no corresponding RFXCom message was found (invalid command)
   //--------------------------------------------------------------
   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> createRfxcomMessage(boost::shared_ptr<yApi::IDeviceCommand> command) const;

   //--------------------------------------------------------------
   /// \brief	                     Get the RFXCom type as string
   /// \param [in] rfxcomType       The raw coded value
   /// \return                      RFXCom type as string
   //--------------------------------------------------------------
   static const std::string rfxcomTypeToString(const unsigned char rfxcomType);

   //--------------------------------------------------------------
   /// \brief	                     Trace the RFXCom configured protocols
   /// \param [in] rbuf             The raw received message (must be IRESPONSE subtype)
   //--------------------------------------------------------------
   static void TraceRfxComConfiguredProtocols(const RBUF& rbuf);

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
   /// \brief  The request structure (keep here for better performance)
   //--------------------------------------------------------------
   RBUF m_request;

   //--------------------------------------------------------------
   /// \brief  The answer structure (keep here for better performance)
   //--------------------------------------------------------------
   RBUF m_answer;
};



