#pragma once
#include <shared/plugin/IPlugin.h>
#include "RfxcomConfiguration.h"
#include "Transceiver.h"
#include <shared/communication/IAsyncPort.h>
#include "rfxcomMessages/Ack.h"
#include "rfxcomMessages/TransceiverStatus.h"
#include <shared/communication/BufferLogger.h>
#include <shared/DataContainer.h>
#include <shared/event/EventTimer.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This plugin support the RFXCom module (see http://rfxcom.com/)
//--------------------------------------------------------------
class CRfxcom : public shared::plugin::IPlugin
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CRfxcom();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcom();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYadomsApi> context);
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a message to the RFXCom
   /// \param [in] buffer           Buffer to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   //--------------------------------------------------------------
   void send(const shared::communication::CByteBuffer& buffer, bool needAnswer = false);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] command          The received command (JSON string)
   /// \param [in] deviceParameters The device parameters (JSON string)
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer & deviceParameters);

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes connected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes unconnected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received by the RFXCom
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] data             Data received
   //--------------------------------------------------------------
   void processRfxcomDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const shared::communication::CByteBuffer& data);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the RFXCom
   /// \param [in] eventHandler     Event handler to be notified on events on the connection
   //--------------------------------------------------------------
   void createConnection(shared::event::CEventHandler& eventHandler);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the RFXCom
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Initialize the connected RFXCom
   //--------------------------------------------------------------
   void initRfxcom();

   //--------------------------------------------------------------
   /// \brief	                     Process error (disconnect and retry connect later)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void errorProcess(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Process received response to command from RFXCom
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] status           Received status
   //--------------------------------------------------------------
   void processRfxcomCommandResponseMessage(boost::shared_ptr<yApi::IYadomsApi> context, const rfxcomMessages::CTransceiverStatus& status);

   //--------------------------------------------------------------
   /// \brief	                     Process status message from RFXCom
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] status           Received status
   //--------------------------------------------------------------
   void processRfxcomStatusMessage(boost::shared_ptr<yApi::IYadomsApi> context, const rfxcomMessages::CTransceiverStatus& status);

   //--------------------------------------------------------------
   /// \brief	                     Process wrong command response from RFXCom
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] status           Received status
   //--------------------------------------------------------------
   void processRfxcomWrongCommandMessage(boost::shared_ptr<yApi::IYadomsApi> context, const rfxcomMessages::CTransceiverStatus& status);

   //--------------------------------------------------------------
   /// \brief	                     Process received ack message from RFXCom
   /// \param [in] status           Received acknowledge
   //--------------------------------------------------------------
   void processRfxcomAckMessage(const rfxcomMessages::CAck& ack) const;

   //--------------------------------------------------------------
   /// \brief	                     Check if connections are the same between the 2 configurations
   /// \param [in] conf1            First configuration to compare
   /// \param [in] conf2            Second configuration to compare
   /// \return                      true is connection data are all the same in the both configurations
   //--------------------------------------------------------------
   bool connectionsAreEqual(const CRfxcomConfiguration& conf1, const CRfxcomConfiguration& conf2) const;

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CRfxcomConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The RFXCom protocol implementation object
   //--------------------------------------------------------------
   boost::shared_ptr<ITransceiver> m_transceiver;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   shared::communication::CBufferLogger m_logger;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	The state machine
   //--------------------------------------------------------------
   enum EStateMachine
   {
      kNotInitialized = 0,    // RFXCom not initialized
      kResettingRfxcom,       // Resetting RFXCom is in progress
      kGettingRfxcomStatus,   // Getting the current RFXCom status (active protocols, etc...)
      kSettingRfxcomMode,     // Update the RFXCom mode (active protocols, etc...)
      kRfxcomIsRunning        // RFXCom is fully initialized, and is running
   };
   mutable EStateMachine m_currentState;
};



