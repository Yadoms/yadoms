#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "TeleInfoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "IDecoder.h"
#include <shared/communication/AsyncPortConnectionNotification.h>
#include "TeleInfoReceiveBufferHandler.h"
#include "FT2xxSerialPort.h"
#include "IGPIOManager.h"

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
   /// \param [in] messages         The buffer with the received information
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const boost::shared_ptr<std::map<std::string, std::string>>& messages);

   //--------------------------------------------------------------
   /// \brief	                     Called when the TeleInfo Receiver becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processTeleInfoConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	                     Called when the TeleInfo Receiver becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] notification     The connection notification data
   //--------------------------------------------------------------
   void processTeleInfoUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification = boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>());

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
   boost::shared_ptr<shared::communication::CFT2xxSerialPort> m_port;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	Periodic sampling event
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_periodicSamplingTimer;

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   boost::shared_ptr<CTeleInfoConfiguration> m_configuration;

   //--------------------------------------------------------------
   /// \brief	The TeleInfo protocol implementation object
   //--------------------------------------------------------------
   boost::shared_ptr<IDecoder> m_decoder[2];

   //--------------------------------------------------------------
   /// \brief  The receiver buffer
   //--------------------------------------------------------------
   boost::shared_ptr<CTeleInfoReceiveBufferHandler> m_receiveBufferHandler;

   //--------------------------------------------------------------
   /// \brief  developerMode state
   //--------------------------------------------------------------
   bool m_isDeveloperMode;

   //-----------------------------------------------------
   ///\brief The plugin state
   //-----------------------------------------------------

   enum ETeleInfoPluginState
   {
      kUndefined = 0,
      kStop,
      kConnecting,
      kConnectionLost,
      kErDFCounterdesactivated,
      kupdateConfiguration,
      kRunning
   };

   //--------------------------------------------------------------
   /// \brief	The plugin state
   //--------------------------------------------------------------
   ETeleInfoPluginState m_runningState;

   //--------------------------------------------------------------
   /// \brief	    List of port to read
   //--------------------------------------------------------------
   boost::shared_ptr<IGPIOManager> m_GPIOManager;
};