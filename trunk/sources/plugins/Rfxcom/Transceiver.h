#pragma once

#include "ITransceiver.h"
#include "IPort.h"
#include "TransceiverStatus.h"
#include "rfxcomMessages/IRfxcomMessage.h"
#include "rfxcomMessages/RFXtrxHelpers.h"
#include "ISequenceNumberProvider.h"
#include "IRfxcomConfiguration.h"

//--------------------------------------------------------------
/// \brief	This class implement the RFXCom protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] port                    Port used to exchange messages
   //--------------------------------------------------------------
   CTransceiver(const IRfxcomConfiguration& configuration, boost::shared_ptr<IPort> port);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   virtual void processReset();
   virtual void send(const std::string& command, const std::string& deviceParameters);
   virtual void historize(boost::shared_ptr<yApi::IYadomsApi> context, const CByteBuffer& data) const;
   // [END] ITransceiver implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Build the command message for the transceiver
   /// \param [in] command          Command type
   //--------------------------------------------------------------
   CByteBuffer buildRfxcomCommand(unsigned char command) const;

   //--------------------------------------------------------------
   /// \brief	                     Build the set mode command message for the transceiver
   /// \param [in] frequency        Frequency to set
   /// \param [in] configuration    Mode configuration
   //--------------------------------------------------------------
   CByteBuffer buildRfxcomSetModeCommand(unsigned char frequency, const IRfxcomConfiguration& configuration) const;

   //--------------------------------------------------------------
   /// \brief	                     Send a command to RFXCom and check answer (status message)
   /// \param [in] cmd              The command to send
   /// \return                      RfxCom status
   //--------------------------------------------------------------
   CTransceiverStatus sendCommand(const CByteBuffer& cmd);

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

private:
   //--------------------------------------------------------------
   /// \brief  The plugin configuration
   //--------------------------------------------------------------
   const IRfxcomConfiguration& m_configuration;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<IPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;
};



