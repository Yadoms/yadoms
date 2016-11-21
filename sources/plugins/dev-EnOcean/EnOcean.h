#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "EnOceanConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "message/ReceivedMessage.h"
#include "message/SendMessage.h"
#include "ProfileHelper.h"
#include "IEnOceanReceiveThread.h"
#include "IMessageHandler.h"


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
   /// \brief	                     Load all devices (create device object from devices stored in database)
   //--------------------------------------------------------------
   void loadAllDevices();

   //--------------------------------------------------------------
   /// \brief	                     Process a command received from Yadoms
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   void processDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command);

   //--------------------------------------------------------------
   /// \brief	                     Called when dongle becomes connected
   //--------------------------------------------------------------
   void processConnectionEvent();

   //--------------------------------------------------------------
   /// \brief	                     Called when dongle becomes unconnected
   //--------------------------------------------------------------
   void processUnConnectionEvent();

   //--------------------------------------------------------------
   /// \brief	                     Called when device manually creation is invoked
   /// \return                      The declared device name
   //--------------------------------------------------------------
   std::string processManuallyDeviceCreation(boost::shared_ptr<const shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> creation);

   //--------------------------------------------------------------
   /// \brief	                     Called when device was removed
   /// \param[in] deviceRemoved     Information about device removed
   //--------------------------------------------------------------
   void processDeviceRemmoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> deviceRemoved);

   //--------------------------------------------------------------
   /// \brief	                     Called when the device configuration was updated by user
   /// \param [in] setDeviceConfigurationData The new device configuration data
   //--------------------------------------------------------------
   void processDeviceConfiguration(boost::shared_ptr<const yApi::ISetDeviceConfiguration> setDeviceConfigurationData);

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the UPS
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(const message::CEsp3ReceivedPacket& message);

   //--------------------------------------------------------------
   /// \brief	                     Process received messages
   /// \param [in] esp3Packet       Message received
   //--------------------------------------------------------------
   void processRadioErp1(const message::CEsp3ReceivedPacket& esp3Packet);
   static void processResponse(const message::CEsp3ReceivedPacket& esp3Packet);
   void processDongleVersionResponse(const message::CEsp3ReceivedPacket& esp3Packet) const;
   static void processEvent(const message::CEsp3ReceivedPacket& esp3Packet);

   //--------------------------------------------------------------
   /// \brief	                           Declare a device
   /// \param [in] deviceId               Device ID
   /// \param [in] profile                Device profile
   /// \param [in] manufacturer           Manufacturer
   /// \param [in] model                  Device model (auto-generated if not provided)
   /// \return                            The created device
   //--------------------------------------------------------------
   boost::shared_ptr<IType> declareDevice(const std::string& deviceId,
                                          const CProfileHelper& profile,
                                          const std::string& manufacturer,
                                          const std::string& model = std::string());

   //--------------------------------------------------------------
   /// \brief	                     Declare a device when ignoring profile
   /// \param [in] deviceId         Device ID to declare
   //--------------------------------------------------------------
   void declareDeviceWithoutProfile(const std::string& deviceId) const;


   //--------------------------------------------------------------
   /// \brief	                     Extract sender ID from buffer
   /// \param [in] data             Buffer containing sender ID
   /// \param [in] startIndex       Index of the first byte
   //--------------------------------------------------------------
   static std::string extractSenderId(const std::vector<unsigned char>& data,
                                      int startIndex);//TODO virer

   //--------------------------------------------------------------
   /// \brief	                     Requests to EnOcean
   //--------------------------------------------------------------
   void requestDongleVersion() const;

   //--------------------------------------------------------------
   /// \brief	                     Create the connection
   //--------------------------------------------------------------
   void createConnection();

   //--------------------------------------------------------------
   /// \brief	                     Close the connection
   //--------------------------------------------------------------
   void destroyConnection();

   //--------------------------------------------------------------
   /// \brief	                     Protocol error processing (retry last command)
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

   //--------------------------------------------------------------
   /// \brief	                     Create a new device (local only)
   /// \param [in] deviceId         The device ID
   /// \param [in] profileHelper    The device profile
   /// \return                      New device created
   //--------------------------------------------------------------
   boost::shared_ptr<IType> createDevice(const std::string& deviceId,
                                         const CProfileHelper& profileHelper) const;

   //--------------------------------------------------------------
   /// \brief	                     Generate a default model name if none provided
   /// \param [in] model            Original model name
   /// \param [in] manufacturer     Manufacturer name
   /// \param [in] profile          Device profile
   /// \return                      The model name
   //--------------------------------------------------------------
   std::string generateModel(const std::string& model,
                             const std::string& manufacturer,
                             const CProfileHelper& profile) const;

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
   /// \brief  The receiver thread
   //--------------------------------------------------------------
   boost::shared_ptr<IEnOceanReceiveThread> m_receiverThread;

   //--------------------------------------------------------------
   /// \brief  The message handler
   //--------------------------------------------------------------
   boost::shared_ptr<IMessageHandler> m_messageHandler;

   //--------------------------------------------------------------
   /// \brief  Api access
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::IYPluginApi> m_api;

   //--------------------------------------------------------------
   /// \brief  The communication port logger
   //--------------------------------------------------------------
   shared::communication::CAsciiBufferLogger m_logger;

   //--------------------------------------------------------------
   /// \brief  The known devices list
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<IType>> m_devices;
};

