#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "NitramConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/IBufferLogger.h>
#include <shared/event/EventTimer.h>
#include <shared/communication/AsyncPortConnectionNotification.h>
#include "NitramMessages/BatteryCapacityAndVoltage.h"
#include "NitramMessages/Runtime.h"
#include "NitramMessages/SetupShutdownDelay.h"
#include "NitramMessages/SetupStartupDelay.h"
#include "NitramMessages/SummaryStatus.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This plugin supports Nitram Protocol
//--------------------------------------------------------------
class CNitram : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CNitram();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CNitram();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a buffer to the UPS
   /// \param [in] buffer           Buffer to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   /// \param [in] answerIsRequired true if answer is required (Used for some UPS, not supporting all commands)
   //--------------------------------------------------------------
   void send(const boost::shared_ptr<nitramMessages::INitramMessage> message);

   //--------------------------------------------------------------
   /// \brief	                     Called when the UPS becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Called when the UPS becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] notification     The connection notification data
   //--------------------------------------------------------------
   void processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                                 boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification = boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>());

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the UPS
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const shared::communication::CByteBuffer& buffer);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the UPS
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void createConnection(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Close the connection to the UPS
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Protocol error processing (retry last command)
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Check if connections are the same between the 2 configurations
   /// \param [in] conf1            First configuration to compare
   /// \param [in] conf2            Second configuration to compare
   /// \return                      true is connection data are all the same in the both configurations
   //--------------------------------------------------------------
   static bool connectionsAreEqual(const CNitramConfiguration& conf1,
                                   const CNitramConfiguration& conf2);

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Get status' command
   //--------------------------------------------------------------
   void sendGetStatusCmd();

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Toggle beep' command
   //--------------------------------------------------------------
   void sendGetRuntimeCmd();

   void sendGetBatteryVoltageCmd();

   void onCommandShutdown(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command);

   void onCommandRestore(boost::shared_ptr<yApi::IYPluginApi> api,
                         const std::string& command);

   void ChangeShutdownDelay(boost::shared_ptr<yApi::IYPluginApi> api, 
                            boost::posix_time::time_duration delay);

   void ChangeRestoreDelay(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::posix_time::time_duration delay);

   //--------------------------------------------------------------
   /// \brief	                     declare device (and associated keywords)
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] model            The UPS model
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& model);

   void sendAck();
   void send();

private:

   //-----------------------------------------------------
   ///\brief The plugin state
   //-----------------------------------------------------

   enum ENitramPluginState
   {
      kUndefined = 0,
      kStop,
      kInitializationError,
      kupdateConfiguration,
      kConnecting,
      kNoConnection,
      kRunning
   };

   //--------------------------------------------------------------
   /// \brief Manage the state of the plugin
   /// \param[in] newState  the new state
   //--------------------------------------------------------------
   void setPluginState(boost::shared_ptr<yApi::IYPluginApi> api, ENitramPluginState newState);

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CNitramConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The communication port logger
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IBufferLogger> m_logger;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	The delay between 2 UPS status request
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_upsStatusRequestTimer;

   //--------------------------------------------------------------
   /// \brief	The delay between 2 UPS status request
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_upsBatteryVoltageRequest;

   //--------------------------------------------------------------
   /// \brief	The communication error retry counter
   //--------------------------------------------------------------
   unsigned int m_protocolErrorCounter;

   //--------------------------------------------------------------
   /// \brief	The last sent buffer (used for retries)
   //--------------------------------------------------------------
   boost::shared_ptr<nitramMessages::INitramMessage> m_lastSentBuffer;

   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   static const std::string DeviceName;

   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   static const std::string Model;

   //--------------------------------------------------------------
   /// \brief	AC power status
   //--------------------------------------------------------------
   bool m_acPowerActive;

   //--------------------------------------------------------------
   /// \brief	The AC power state
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_acPowerHistorizer;

   boost::shared_ptr<nitramMessages::CBatteryCapacityAndVoltage> m_messageBatteryAndVoltage;
   boost::shared_ptr<nitramMessages::CSummaryStatus> m_messageSummaryStatus;
   boost::shared_ptr<nitramMessages::CRuntime> m_messageRuntime;
   boost::shared_ptr<nitramMessages::CSetupShutdownDelay> m_messageShutdownDelay;
   boost::shared_ptr<nitramMessages::CSetupStartupDelay> m_messageStartupDelay;

   //--------------------------------------------------------------
   /// \brief	The shutdown device
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CEvent> m_upsShutdown;

   //--------------------------------------------------------------
   /// \brief	The restore device
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CEvent> m_upsRestore;

   //--------------------------------------------------------------
   /// \brief	The keywords list to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   //--------------------------------------------------------------
   /// \brief	Buffers Queue
   //--------------------------------------------------------------
   std::queue<boost::shared_ptr<nitramMessages::INitramMessage>> buffers;

   //--------------------------------------------------------------
   /// \brief	The plugin state
   //--------------------------------------------------------------
   ENitramPluginState m_runningState;
};