#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "RfxcomFactory.h"
#include "RfxcomConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "rfxcomMessages/Ack.h"
#include "rfxcomMessages/TransceiverStatus.h"
#include <shared/communication/BufferLogger.h>
#include <shared/DataContainer.h>
#include <shared/event/EventTimer.h>
#include <shared/communication/AsyncPortConnectionNotification.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This plugin support the RFXCom module (see http://rfxcom.com/)
//--------------------------------------------------------------
class CRfxcom : public plugin_cpp_api::IPlugin
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
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a message to the RFXCom
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] buffer           Buffer to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   //--------------------------------------------------------------
   void send(boost::shared_ptr<yApi::IYPluginApi> api,
             const shared::communication::CByteBuffer& buffer,
             bool needAnswer);

   //--------------------------------------------------------------
   /// \brief	                    Send several messages to the RFXCom
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] buffers          Buffers to send
   //--------------------------------------------------------------
   void send(boost::shared_ptr<yApi::IYPluginApi> api,
             boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> buffers);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  boost::shared_ptr<const yApi::IDeviceCommand> command);

   //--------------------------------------------------------------
   /// \brief	                     Process a configuration update
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] newConfigurationData The new configuration data
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processRfxcomConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Called when the RFXCom becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] notification     The connection notification data
   //--------------------------------------------------------------
   void processRfxcomUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification = boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>());

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received by the RFXCom
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] data             Data received
   //--------------------------------------------------------------
   void processRfxcomDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const shared::communication::CByteBuffer& data);

   //--------------------------------------------------------------
   /// \brief	                     Process the firmware update
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] extraQuery       Extra query
   //--------------------------------------------------------------
   void processFirmwareUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                              boost::shared_ptr<yApi::IExtraQuery> extraQuery);

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
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void initRfxcom(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Process error (disconnect and retry connect later)
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void errorProcess(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Process received response to command from RFXCom
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] status           Received status
   //--------------------------------------------------------------
   void processRfxcomCommandResponseMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const rfxcomMessages::CTransceiverStatus& status);

   //--------------------------------------------------------------
   /// \brief	                     Process status message from RFXCom
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] status           Received status
   //--------------------------------------------------------------
   void processRfxcomStatusMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const rfxcomMessages::CTransceiverStatus& status);

   //--------------------------------------------------------------
   /// \brief	                     Process receiver started message from RFXCom
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] status           Received status
   //--------------------------------------------------------------
   void processRfxcomReceiverStartedMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const rfxcomMessages::CTransceiverStatus& status);

   //--------------------------------------------------------------
   /// \brief	                     Process wrong command response from RFXCom
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] status           Received status
   //--------------------------------------------------------------
   void processRfxcomWrongCommandMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const rfxcomMessages::CTransceiverStatus& status);

   //--------------------------------------------------------------
   /// \brief	                     Process received ack message from RFXCom
   /// \param [in] status           Received acknowledge
   //--------------------------------------------------------------
   static void processRfxcomAckMessage(const rfxcomMessages::CAck& ack);

   //--------------------------------------------------------------
   /// \brief	                     Check if connections are the same between the 2 configurations
   /// \param [in] conf1            First configuration to compare
   /// \param [in] conf2            Second configuration to compare
   /// \return                      true is connection data are all the same in the both configurations
   //--------------------------------------------------------------
   static bool connectionsAreEqual(const CRfxcomConfiguration& conf1,
                                   const CRfxcomConfiguration& conf2);

private:
   CRfxcomFactory m_factory;

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
   /// \brief  The buffer logger use to log what is send and receive on serial link
   //--------------------------------------------------------------
   shared::communication::CBufferLogger m_logger;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	The state machine
   //--------------------------------------------------------------
   bool m_configurationUpdated;

   //--------------------------------------------------------------
   /// \brief  The last sent message
   //--------------------------------------------------------------
   shared::communication::CByteBuffer m_lastRequest;
};

