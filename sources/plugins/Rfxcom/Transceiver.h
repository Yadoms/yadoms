#pragma once

#include "ITransceiver.h"
#include "IPort.h"
#include "TransceiverStatus.h"
#include "rfxcomMessages/IRfxcomMessage.h"
#include "rfxcomMessages/RFXtrxHelpers.h"
#include "ISequenceNumberProvider.h"
#include "IRfxcomConfiguration.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This class implement the RFXCom protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] context                 Plugin context
   /// \param[in] configuration           Plugin configuration
   /// \param[in] evtPortConnection       Port connection event ID
   /// \param[in] evtPortDataReceived     Data received on port event ID
   //--------------------------------------------------------------
   CTransceiver(boost::shared_ptr<yApi::IYadomsApi> context, const IRfxcomConfiguration& configuration, int evtPortConnection, int evtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   virtual void processReset();
   virtual void send(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters);
   virtual void historize(const CByteBuffer& data) const;
   // [END] ITransceiver implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Reset the RFXCom
   //--------------------------------------------------------------
   void sendReset();

   //--------------------------------------------------------------
   /// \brief	                     Get RFXCom status
   /// \return                      RfxCom status
   //--------------------------------------------------------------
   CTransceiverStatus sendCommandGetStatus();

   //--------------------------------------------------------------
   /// \brief	                     Set RFXCom mode
   /// \param[in] frequency         RFXCom frequency
   /// \param[in] configuration     Protocols activations
   /// \return                      RfxCom status
   //--------------------------------------------------------------
   CTransceiverStatus sendCommandSetMode(unsigned char frequency, const IRfxcomConfiguration& configuration);

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
   const CByteBuffer buildRfxcomMessage(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters) const;

private:
   //--------------------------------------------------------------
   /// \brief	The plugin context
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::IYadomsApi> m_context;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   const IRfxcomConfiguration& m_configuration;

   //--------------------------------------------------------------
   /// \brief	The port connection event ID
   //--------------------------------------------------------------
   const int m_evtPortConnection;

   //--------------------------------------------------------------
   /// \brief	The data received on port event ID
   //--------------------------------------------------------------
   const int m_evtPortDataReceived;

   //--------------------------------------------------------------
   /// \brief	The port logger
   //--------------------------------------------------------------
   boost::shared_ptr<IPortLogger> m_portLogger;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<IPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;
};



