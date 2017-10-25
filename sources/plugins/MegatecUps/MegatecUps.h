#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "MegatecUpsConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/IBufferLogger.h>
#include <shared/event/EventTimer.h>
#include <shared/communication/AsyncPortConnectionNotification.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This plugin supports Megatec UPS (see http://www.networkupstools.org/protocols/megatec.html)
//--------------------------------------------------------------
class CMegatecUps : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CMegatecUps();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CMegatecUps();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a ASCII message to the UPS
   /// \param [in] message          message to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   /// \param [in] answerIsRequired true if answer is required (Used for some UPS, not supporting all commands)
   //--------------------------------------------------------------
   void send(const std::string& message,
             bool needAnswer = false,
             bool answerIsRequired = true);

   //--------------------------------------------------------------
   /// \brief	                     Send a buffer to the UPS
   /// \param [in] buffer           Buffer to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   /// \param [in] answerIsRequired true if answer is required (Used for some UPS, not supporting all commands)
   //--------------------------------------------------------------
   void send(const shared::communication::CByteBuffer& buffer,
             bool needAnswer = false,
             bool answerIsRequired = true);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommandShutdown(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command);

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
                            const std::string& message);

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
   static bool connectionsAreEqual(const CMegatecUpsConfiguration& conf1,
                                   const CMegatecUpsConfiguration& conf2);

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Get Information' command
   //--------------------------------------------------------------
   void sendGetInformationCmd();

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Get rating Information' command
   //--------------------------------------------------------------
   void sendGetRatingInformationCmd();

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Get status' command
   //--------------------------------------------------------------
   void sendGetStatusCmd();

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Toggle beep' command
   //--------------------------------------------------------------
   void sendToggleBeepCmd();

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Shutdown' command
   //--------------------------------------------------------------
   void sendShtudownCmd();

   //--------------------------------------------------------------
   /// \brief	                     Process received status from UPS
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] tokens           Separated fields (input voltage, input fault voltage, output voltage, etc...)
   //--------------------------------------------------------------
   void processReceivedStatus(boost::shared_ptr<yApi::IYPluginApi> api,
                              const boost::tokenizer<boost::char_separator<char>>& tokens);

   //--------------------------------------------------------------
   /// \brief	                     Process received information from UPS
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] tokens           Separated fields (company name, UPS model, version)
   //--------------------------------------------------------------
   void processReceivedInformation(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const boost::tokenizer<boost::char_separator<char>>& tokens) const;

   //--------------------------------------------------------------
   /// \brief	                     Process received rating information from UPS
   /// \param [in] tokens           Separated fields (voltage, current, battery voltage, frequency)
   //--------------------------------------------------------------
   void processReceivedRatingInformation(const boost::tokenizer<boost::char_separator<char>>& tokens);

   //--------------------------------------------------------------
   /// \brief	                     Convert a string to a double, not depend on locale
   /// \param [in] str              Input value as string
   /// \return                      Converted value as double
   /// \note                        We can not use boot::lexical_cast, as float numbers separator depends on locale
   //--------------------------------------------------------------
   static double upsStr2Double(const std::string& str);

   //--------------------------------------------------------------
   /// \brief	                     declare device (and associated keywords)
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] model            The UPS model
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& model) const;

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CMegatecUpsConfiguration m_configuration;

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
   /// \brief	The communication error retry counter
   //--------------------------------------------------------------
   unsigned int m_protocolErrorCounter;

   //--------------------------------------------------------------
   /// \brief	The last sent buffer (used for retries)
   //--------------------------------------------------------------
   shared::communication::CByteBuffer m_lastSentBuffer;

   //--------------------------------------------------------------
   /// \brief	Flag if the answer for the last sent command can be omitted
   /// (some UPS don't support all commands, like identification 'I' command)
   //--------------------------------------------------------------
   bool m_answerIsRequired;

   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   static const std::string DeviceName;

   //--------------------------------------------------------------
   /// \brief	AC power status
   //--------------------------------------------------------------
   bool m_acPowerActive;

   //--------------------------------------------------------------
   /// \brief	The battery nominal voltage (V)
   //--------------------------------------------------------------
   double m_batteryNominalVoltage;

   //--------------------------------------------------------------
   /// \brief	The input voltage (V)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CVoltage> m_inputVoltage;

   //--------------------------------------------------------------
   /// \brief	The input fault voltage (V)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CVoltage> m_inputfaultVoltage;

   //--------------------------------------------------------------
   /// \brief	The output voltage (V)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CVoltage> m_outputVoltage;

   //--------------------------------------------------------------
   /// \brief	The output load (%)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CLoad> m_outputLoad;

   //--------------------------------------------------------------
   /// \brief	The input frequency (Hz)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CFrequency> m_inputFrequency;

   //--------------------------------------------------------------
   /// \brief	The battery voltage (V)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CVoltage> m_batteryVoltage;

   //--------------------------------------------------------------
   /// \brief	The temperature (°C)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;

   //--------------------------------------------------------------
   /// \brief	The AC power state
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_acPowerHistorizer;

   //--------------------------------------------------------------
   /// \brief	The battery low state
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_batteryLowHistorizer;

   //--------------------------------------------------------------
   /// \brief	The shutdown device
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CEvent> m_upsShutdown;

   //--------------------------------------------------------------
   /// \brief	The keywords list to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   //--------------------------------------------------------------
   /// \brief	Boolean states cache
   //--------------------------------------------------------------
   boost::optional<bool> m_lastBatteryLowState, m_acPowerState;
};

