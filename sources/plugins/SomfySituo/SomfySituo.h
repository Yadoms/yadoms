#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "SomfySituoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/IBufferLogger.h>
#include <shared/event/EventTimer.h>
#include <shared/communication/AsyncPortConnectionNotification.h>
#include "SomfyRemoteControl.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//-----------------------------------------------------
///\brief The security mode for sending emails
//-----------------------------------------------------
DECLARE_ENUM_HEADER(EChannel,
   ((Channel1)(1))
   ((Channel2)(2))
   ((Channel3)(3))
   ((Channel4)(4))
   ((Channel5)(5))
);

//--------------------------------------------------------------
/// \brief	This plugin supports adapter for Somfy Remote Controls)
//--------------------------------------------------------------
class CSomfySituo : public plugin_cpp_api::IPlugin
{
public:
   typedef enum
   {
      Debug = 'D',
   } ConfigSituo;

   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CSomfySituo();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CSomfySituo();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

protected:

   //--------------------------------------------------------------
   /// \brief	                     manage all incoming events
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void manageEvents(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Send a ASCII message to the Controler
   /// \param [in] message          message to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   //--------------------------------------------------------------
   void send(const std::string& message,
             bool needAnswer = false);

   //--------------------------------------------------------------
   /// \brief	                     Send a buffer to the Controler
   /// \param [in] buffer           Buffer to send
   /// \param [in] needAnswer       true if answer is needed. If true, a timeout will occur if no answer is received.
   //--------------------------------------------------------------
   void send(const shared::communication::CByteBuffer& buffer,
             bool needAnswer = false);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] device           the device which received the command
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& device, const std::string& command);

   //--------------------------------------------------------------
   /// \brief	                     Called when the remote Control becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Called when the Remote Control becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] notification     The connection notification data
   //--------------------------------------------------------------
   void processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                                 boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification
                                    =
                                    boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>());


   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the remote Control
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                            const std::string& message);

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the remote Control
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void createConnection(boost::shared_ptr<yApi::IYPluginApi> api);

   //--------------------------------------------------------------
   /// \brief	                     Close the connection to the Remote Control
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
   static bool connectionsAreEqual(const CSomfySituoConfiguration& conf1,
                                   const CSomfySituoConfiguration& conf2);

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Config' command
   //--------------------------------------------------------------
   void sendConfigCmd(ConfigSituo conf, int value);

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Get status' command
   //--------------------------------------------------------------
   void sendGetStatusCmd();

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Channel' command
   //--------------------------------------------------------------
   void sendChannelCmd();

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Up' command
   //--------------------------------------------------------------
   void sendUpCmd();
   void sendQuickUpCmd(int chan);

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Down' command
   //--------------------------------------------------------------
   void sendDownCmd();
   void sendQuickDownCmd(int chan);

   //--------------------------------------------------------------
   /// \brief	                     Send the 'My' command
   //--------------------------------------------------------------
   void sendMyCmd();
   void sendQuickMyCmd(int chan);

   //--------------------------------------------------------------
   /// \brief	                     Send the 'Prog' command
   //--------------------------------------------------------------
   void sendProgCmd();
   void sendQuickProgCmd(int chan);

   //--------------------------------------------------------------
   /// \brief	                     Process received information from Controler
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] tokens           Separated fields (company name, Controler model, version)
   //--------------------------------------------------------------
   void processReceivedInformation(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const boost::tokenizer<boost::char_separator<char>>& tokens) const;

   //--------------------------------------------------------------
   /// \brief	                     declare device (and associated keywords)
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] model            The Controler model
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& model, const std::string& version) const;

private:

   //--------------------------------------------------------------
   /// \brief	The plugin configuration
   //--------------------------------------------------------------
   CSomfySituoConfiguration m_configuration;

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
   /// \brief	The communication error retry counter
   //--------------------------------------------------------------
   unsigned int m_protocolErrorCounter;

   //--------------------------------------------------------------
   /// \brief	The last sent buffer (used for retries)
   //--------------------------------------------------------------
   shared::communication::CByteBuffer m_lastSentBuffer;


   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   static const std::string DeviceName;

   //--------------------------------------------------------------
   /// \brief	the active channel
   //--------------------------------------------------------------
   int m_activeChannel;
   //--------------------------------------------------------------
   /// \brief	Indicate whether the channel is lit or not
   //--------------------------------------------------------------
   bool m_channelSleep;


   //--------------------------------------------------------------
   /// \brief	    Current values of the remote control
   //--------------------------------------------------------------	
   boost::shared_ptr<yApi::historization::CCurtain> m_curtain;

   //--------------------------------------------------------------
   /// \brief  The receiver buffer
   //--------------------------------------------------------------
   boost::shared_ptr<CSomfySituoReceiveBufferHandler> m_ReceiveBufferHandler;

   //--------------------------------------------------------------
   /// \brief	The keywords list to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   static const std::map<std::string, int> m_somfyModels;
};
