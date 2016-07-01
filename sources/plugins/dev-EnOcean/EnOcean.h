#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "EnOceanConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include <shared/event/EventTimer.h>
#include "EnOceanMessage.h"


class CMegatecUpsConfiguration;
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This plugin supports EnOcean
//--------------------------------------------------------------
class CEnOcean : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CEnOcean();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CEnOcean();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:
   //TOTO tout ce qui suit
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
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command);


   //--------------------------------------------------------------
   /// \brief	                     Called when the UPS becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Called when the UPS becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the UPS
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const EnOceanMessage::CMessage& message);

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
   static bool connectionsAreEqual(const CEnOceanConfiguration& conf1,
                                   const CEnOceanConfiguration& conf2);

private:
   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CEnOceanConfiguration m_configuration;

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
};

