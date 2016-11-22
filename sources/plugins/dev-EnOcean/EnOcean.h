#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "Configuration.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "message/ReceivedMessage.h"
#include "ProfileHelper.h"
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
   /// \brief	                     Reconfigure a device (configuration must be set in Yadoms first)
   /// \param [in] deviceId         The device ID
   /// \param [in] configuration    The device configuration
   //--------------------------------------------------------------
   void processDeviceConfiguration(const std::string& deviceId,
                                   const shared::CDataContainer& configuration);

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the UPS
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> message);

   //--------------------------------------------------------------
   /// \brief	                     Process received messages
   /// \param [in] esp3Packet       Message received
   //--------------------------------------------------------------
   void processRadioErp1(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);
   static void processResponse(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);
   static void processDongleVersionResponse(boost::shared_ptr<const message::CEsp3ReceivedPacket> dongleVersionResponse);
   static void processEvent(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);

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
   static bool connectionsAreEqual(const CConfiguration& conf1,
                                   const CConfiguration& conf2);

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
   CConfiguration m_configuration;

   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The message handler
   //--------------------------------------------------------------
   boost::shared_ptr<IMessageHandler> m_messageHandler;

   //--------------------------------------------------------------
   /// \brief  Api access
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::IYPluginApi> m_api;

   //--------------------------------------------------------------
   /// \brief  The known devices list (only configured devices)
   //--------------------------------------------------------------
   std::map<std::string, boost::shared_ptr<IType>> m_devices;
};

