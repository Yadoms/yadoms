#pragma once
#include <shared/plugin/IPlugin.h>
#include "TeleInfoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/BufferLogger.h>
#include "Transceiver.h"
#include "TransceiverStatus.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

#define TeleInfoMESSAGE_maxSize 16

//--------------------------------------------------------------
/// \brief	This class is the TeleInfo plugin
/// \note   This plugin receive information from a specific USB module that read some security devices from TeleInfo
//--------------------------------------------------------------
class CTeleInfo : public shared::plugin::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CTeleInfo();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CTeleInfo();

   //--------------------------------------------------------------
   /// \brief	                    Send a message to the TeleInfo Receiver
   /// \param [in] buffer           Buffer to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   //--------------------------------------------------------------
   void send(const shared::communication::CByteBuffer& buffer, bool needAnswer = false);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   /// \param [in] eventHandler     Event handler to be notified on events on the connection
   //--------------------------------------------------------------
   void createConnection(shared::event::CEventHandler& eventHandler);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> context, const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> context, 
							const shared::communication::CByteBuffer& data);

   //--------------------------------------------------------------
   /// \brief	                     Called when the TeleInfo Receiver becomes connected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processTeleInfoConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Called when the TeleInfo Receiver becomes unconnected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processTeleInfoUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void initTeleInfoReceiver();

   // IPlugin implementation
   virtual void doWork(boost::shared_ptr<yApi::IYPluginApi> context);
   // [END] IPlugin implementation

private:
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CTeleInfoConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief	The TeleInfo protocol implementation object
   //--------------------------------------------------------------
   boost::shared_ptr<ITransceiver> m_transceiver;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   shared::communication::CBufferLogger m_logger;
};