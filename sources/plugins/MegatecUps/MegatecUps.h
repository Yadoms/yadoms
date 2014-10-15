#pragma once
#include <shared/plugin/IPlugin.h>
#include "MegatecUpsConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include <shared/DataContainer.h>
#include <shared/event/EventTimer.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This plugin supports Megatec UPS (see http://www.networkupstools.org/protocols/megatec.html)
//--------------------------------------------------------------
class CMegatecUps : public shared::plugin::IPlugin
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
   virtual void doWork(boost::shared_ptr<yApi::IYadomsApi> context);
   // [END] IPlugin implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a message to the UPS
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
   /// \brief	                     Called when the UPS becomes connected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Called when the UPS becomes unconnected
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processUnConnectionEvent(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the UPS
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& message);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the UPS
   /// \param [in] eventHandler     Event handler to be notified on events on the connection
   //--------------------------------------------------------------
   void createConnection(shared::event::CEventHandler& eventHandler);

   //--------------------------------------------------------------
   /// \brief	                     Close the connection to the UPS
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Process error (disconnect and retry connect later)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void errorProcess(boost::shared_ptr<yApi::IYadomsApi> context);

   //--------------------------------------------------------------
   /// \brief	                     Check if connections are the same between the 2 configurations
   /// \param [in] conf1            First configuration to compare
   /// \param [in] conf2            Second configuration to compare
   /// \return                      true is connection data are all the same in the both configurations
   //--------------------------------------------------------------
   bool connectionsAreEqual(const CMegatecUpsConfiguration& conf1, const CMegatecUpsConfiguration& conf2) const;

   //--------------------------------------------------------------
   /// \brief	                     Build the 'Get Information' command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   const shared::communication::CByteBuffer buildGetInformationCmd() const;

   //--------------------------------------------------------------
   /// \brief	                     Build the 'Get rating Information' command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   const shared::communication::CByteBuffer buildGetRatingInformationCmd() const;

   //--------------------------------------------------------------
   /// \brief	                     Build the 'Get status' command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   const shared::communication::CByteBuffer buildGetStatusCmd() const;

   //--------------------------------------------------------------
   /// \brief	                     Build the 'Toggle beep' command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   const shared::communication::CByteBuffer buildToggleBeepCmd() const;

   //--------------------------------------------------------------
   /// \brief	                     Process received status from UPS
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] tokens           Separated fields (input voltage, input fault voltage, output voltage, etc...)
   //--------------------------------------------------------------
   void processReceivedStatus(boost::shared_ptr<yApi::IYadomsApi> context, const boost::tokenizer<boost::char_separator<char> >& tokens);

   //--------------------------------------------------------------
   /// \brief	                     Process received information from UPS
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] tokens           Separated fields (company name, UPS model, version)
   //--------------------------------------------------------------
   void processReceivedInformation(boost::shared_ptr<yApi::IYadomsApi> context, const boost::tokenizer<boost::char_separator<char> >& tokens);

   //--------------------------------------------------------------
   /// \brief	                     Process received rating information from UPS
   /// \param [in] tokens           Separated fields (voltage, current, battery voltage, frequency)
   //--------------------------------------------------------------
   void processReceivedRatingInformation(const boost::tokenizer<boost::char_separator<char> >& tokens);

   //--------------------------------------------------------------
   /// \brief	                     Convert a string to a double, not depend on locale
   /// \param [in] str              Input value as string
   /// \return                      Converted value as double
   /// \note                        We can not use boot::lexical_cast, as float numbers separator depends on locale
   //--------------------------------------------------------------
   static double upsStr2Double(const std::string& str);

   //--------------------------------------------------------------
   /// \brief	                     declare device (and associated keywords)
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] model            The UPS model
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& model) const;

   //--------------------------------------------------------------
   /// \brief	                     declare device (and associated keywords)
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;

   //--------------------------------------------------------------
   /// \brief	                     process AC power status
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] acPowerActive    AC power current status
   //--------------------------------------------------------------
   void processAcPowerStatus(boost::shared_ptr<yApi::IYadomsApi> context, bool acPowerActive);

   //--------------------------------------------------------------
   /// \brief	                     notify Yadoms for current power state
   /// \param [in] context          Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void notifyPowerState(boost::shared_ptr<yApi::IYadomsApi> context);

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
   shared::communication::CAsciiBufferLogger m_logger;

   //--------------------------------------------------------------
   /// \brief	Wait for answer timer
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_waitForAnswerTimer;

   //--------------------------------------------------------------
   /// \brief	The delay between 2 UPS status request
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_upsStatusRequestTimer;

   //--------------------------------------------------------------
   /// \brief	The delay before notify Yadoms when AC power is lost
   //--------------------------------------------------------------
   boost::shared_ptr<shared::event::CEventTimer> m_filterTimer;

   //--------------------------------------------------------------
   /// \brief	The input voltage (V)
   //--------------------------------------------------------------
   yApi::historization::CVoltage m_inputVoltage;

   //--------------------------------------------------------------
   /// \brief	The input fault voltage (V)
   //--------------------------------------------------------------
   yApi::historization::CVoltage m_inputfaultVoltage;

   //--------------------------------------------------------------
   /// \brief	The output voltage (V)
   //--------------------------------------------------------------
   yApi::historization::CVoltage m_outputVoltage;

   //--------------------------------------------------------------
   /// \brief	The output current (A)
   //--------------------------------------------------------------
   yApi::historization::CCurrent m_outputCurrent;

   //--------------------------------------------------------------
   /// \brief	The input frequency (Hz)
   //--------------------------------------------------------------
   yApi::historization::CFrequency m_inputFrequency;

   //--------------------------------------------------------------
   /// \brief	The battery voltage (V)
   //--------------------------------------------------------------
   yApi::historization::CVoltage m_batteryVoltage;

   //--------------------------------------------------------------
   /// \brief	The temperature (°C)
   //--------------------------------------------------------------
   yApi::historization::CTemperature m_temperature;

   //--------------------------------------------------------------
   /// \brief	The AC power state
   //--------------------------------------------------------------
   yApi::historization::CSwitch m_acPowerHistorizer;

   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   static const std::string DeviceName;

   //--------------------------------------------------------------
   /// \brief	AC power status
   //--------------------------------------------------------------
   bool m_acPowerActive;
};



