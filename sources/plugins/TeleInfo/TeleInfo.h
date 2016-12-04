#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "TeleInfoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "Transceiver.h"

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

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> context) override;
   // [END] IPlugin implementation
protected:

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the TeleInfo USB Reader
   /// \param [in] context           Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void createConnection(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                        Update the configuration of the plugin
   /// \param [in] context              Plugin execution context (Yadoms API)
   /// \param [in] newConfigurationData The new configuration
   //--------------------------------------------------------------
   void onUpdateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                              const shared::CDataContainer& newConfigurationData);

   //--------------------------------------------------------------
   /// \brief	                     Destroy the serial connection
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Called when received a message from the TeleInfo Receiver
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] data             The buffer with the received information
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const boost::shared_ptr<std::map<std::string, std::string>>& data);

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

   //--------------------------------------------------------------
   /// \brief	                     Process error (disconnect and retry connect later)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void errorProcess(boost::shared_ptr<yApi::IYPluginApi> context);
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
   shared::communication::CAsciiBufferLogger m_logger;

   //--------------------------------------------------------------
   /// \brief  The receiver buffer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IReceiveBufferHandler> m_receiveBufferHandler;

   //--------------------------------------------------------------
   /// \brief  developerMode state
   //--------------------------------------------------------------
   bool m_isDeveloperMode;

   //--------------------------------------------------------------
   /// \brief	The plugin state
   //--------------------------------------------------------------
   bool m_runningState;
};

