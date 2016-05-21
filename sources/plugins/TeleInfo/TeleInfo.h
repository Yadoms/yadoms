#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "TeleInfoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/BufferLogger.h>
#include "Transceiver.h"
#include "TeleInfoReceiveBufferHandler.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the TeleInfo plugin
/// \note   This plugin receive information from a specific USB module that read some security devices from TeleInfo
//--------------------------------------------------------------
class CTeleInfo : public plugin_cpp_api::IPlugin
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
   void send(const shared::communication::CByteBuffer& buffer,
             bool needAnswer = false);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   /// \param [in] eventHandler     Event handler to be notified on events on the connection
   //--------------------------------------------------------------
   void createConnection(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const shared::communication::CByteBuffer& data);

   //--------------------------------------------------------------
   /// \brief	                     Called when the TeleInfo Receiver becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processTeleInfoConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	                     Called when the TeleInfo Receiver becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processTeleInfoUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   //--------------------------------------------------------------
   void initTeleInfoReceiver() const;

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

private:

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

   //--------------------------------------------------------------
   /// \brief  The receiver buffer
   //--------------------------------------------------------------
   boost::shared_ptr<CTeleInfoReceiveBufferHandler> m_receiveBufferHandler;
};

