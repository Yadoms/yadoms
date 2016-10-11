#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "EnOceanConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "message/ReceivedMessage.h"
#include "message/SendMessage.h"
#include "Device.h"

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
   //TODO tout ce qui suit
   //--------------------------------------------------------------
   /// \brief	                     Send a message to EnOcean dongle
   /// \param [in] sendMessage      message to send
   /// \param [in] onReceiveFct     function called when answer is received
   /// \throw                       CProtocolException if timeout waiting answer
   //--------------------------------------------------------------
   void send(const message::CSendMessage& sendMessage) const;
   void send(const message::CSendMessage& sendMessage,
             boost::function<bool(const message::CReceivedEsp3Packet& rcvMessage)> checkExpectedMessageFunction,
             boost::function<void(const message::CReceivedEsp3Packet& rcvMessage)> onReceiveFct);

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void onCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) const;


   //--------------------------------------------------------------
   /// \brief	                     Called when the UPS becomes connected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processConnectionEvent();

   //--------------------------------------------------------------
   /// \brief	                     Called when the UPS becomes unconnected
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void processUnConnectionEvent();

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the UPS
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(const message::CReceivedEsp3Packet& message);

   //--------------------------------------------------------------
   /// \brief	                     Process received messages
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] esp3Packet       Message received
   //--------------------------------------------------------------
   void processRadioErp1(const message::CReceivedEsp3Packet& esp3Packet) const;
   static void processEvent(const message::CReceivedEsp3Packet& esp3Packet);

   //--------------------------------------------------------------
   /// \brief	                     Extract sender ID from buffer
   /// \param [in] data             Buffer containing sender ID
   /// \param [in] startIndex       Index of the first byte
   //--------------------------------------------------------------
   static std::string extractSenderId(const std::vector<unsigned char>& data,
                                      int startIndex);//TODO virer

   //--------------------------------------------------------------
   /// \brief	                     Retrieve device in database
   /// \param [in] deviceId         Device ID to search
   /// \throw std::out_of_range     If device is not found in database
   //--------------------------------------------------------------
   static CDevice retrieveDevice(unsigned int deviceId);

   //--------------------------------------------------------------
   /// \brief	                     Requests to EnOcean
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void requestDongleVersion();

   //--------------------------------------------------------------
   /// \brief	                     Create the connection to the UPS
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void createConnection();

   //--------------------------------------------------------------
   /// \brief	                     Close the connection to the UPS
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Protocol error processing (retry last command)
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void protocolErrorProcess();

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
   /// \brief  Api access
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::IYPluginApi> m_api;

   //--------------------------------------------------------------
   /// \brief  The communication port logger
   //--------------------------------------------------------------
   shared::communication::CAsciiBufferLogger m_logger;

   mutable boost::recursive_mutex m_onReceiveHookMutex;
   boost::function<bool(const message::CReceivedEsp3Packet& rcvMessage)> m_onReceiveHook;
};

