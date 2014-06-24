#pragma once

#include "ITransceiver.h"
#include "IPort.h"
#include "rfxcomMessages/IRfxcomMessage.h"
#include "rfxcomMessages/RFXtrxHelpers.h"
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
   virtual void processReset();
   virtual void send(const std::string& command, const std::string& deviceParameters);
   virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const CByteBuffer& data) const;
   // [END] ITransceiver implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Build the command message to the transceiver
   /// \param [in] command          Command type
   //--------------------------------------------------------------
   CByteBuffer buildRfxcomCommand(unsigned char command);

   //--------------------------------------------------------------
   /// \brief	                     Request the RFXCom status
   /// \return                      true if status received, false if error
   //--------------------------------------------------------------
   bool requestStatus();

   //--------------------------------------------------------------
   /// \brief	                     Check the acknowledge received from RFXCom
   /// \param[in] answer            Buffer received to check
   /// \return                      true if acknowledge is OK, false if any error
   //--------------------------------------------------------------
   bool checkAcknowledge(const CByteBuffer& answer);

   //--------------------------------------------------------------
   /// \brief	                     Create the corresponding RFXCom message associated with the command received from Yadoms
   /// \param [in] command          The received command (JSON string)
   /// \param [in] deviceParameters The device parameters (JSON string)
   /// \return                      RFXCom message to send
   /// \throw shared::exception::CInvalidParameter if no corresponding RFXCom message was found (invalid command)
   //--------------------------------------------------------------
   const CByteBuffer buildRfxcomMessage(const std::string& command, const std::string& deviceParameters) const;

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
};



