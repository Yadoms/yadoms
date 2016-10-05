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
   //TOTO tout ce qui suit
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
   void onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                  boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) const;


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
                            const message::CReceivedEsp3Packet& message);

   //--------------------------------------------------------------
   /// \brief	                     Process received messages
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processRadioErp1(boost::shared_ptr<yApi::IYPluginApi> api,
                         const message::CReceivedEsp3Packet& esp3Packet);
   void processEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                     const message::CReceivedEsp3Packet& esp3Packet);

   //--------------------------------------------------------------
   /// \brief	                     Process radio ERP1 received messages
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] data             Data of the ERP1 message
   //--------------------------------------------------------------
   void processRadioErp1_1BS(boost::shared_ptr<yApi::IYPluginApi> api,
                             const std::vector<unsigned char>& data);

   //--------------------------------------------------------------
   /// \brief	                     Extract sender ID from buffer
   /// \param [in] data             Buffer containing sender ID
   /// \param [in] startIndex       Index of the first byte
   //--------------------------------------------------------------
   static std::string extractSenderId(const std::vector<unsigned char>& data,
                                      int startIndex);//TODO virer

   //--------------------------------------------------------------
   /// \brief	                     Retrieve device in database
   /// \param [in] data             Buffer containing sender ID
   /// \param [in] startIndex       Index of the first byte
   /// \throw std::out_of_range     If device is not found in database
   //--------------------------------------------------------------
   static CDevice retrieveDevice(unsigned int deviceId);

   //--------------------------------------------------------------
   /// \brief	                     Scale a value from a range to another
   /// \param [in] rawValue         Read value from device
   /// \param [in] rangeMin         Min range of the read value (ex 0..255 for a byte value)
   /// \param [in] rangeMax         Max range of the read value
   /// \param [in] scaleMin         Min scale of the real value (ex -40° for temperature sensor of FUNC=2 and Type=1)
   /// \param [in] scaleMax         Max scale of the real value (ex 0° for temperature sensor of FUNC=2 and Type=1)
   /// \return                      Scaled value
   //--------------------------------------------------------------
   static double CEnOcean::scaleToDouble(int rawValue,
                                         int rangeMin,
                                         int rangeMax,
                                         int scaleMin,
                                         int scaleMax);

   //--------------------------------------------------------------
   /// \brief	                     Requests to EnOcean
   /// \param [in] api              Plugin execution context (Yadoms API)
   //--------------------------------------------------------------
   void requestDongleVersion(boost::shared_ptr<yApi::IYPluginApi> api);

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

