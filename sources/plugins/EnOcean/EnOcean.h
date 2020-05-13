#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include "Configuration.h"
#include <shared/communication/IAsyncPort.h>
#include "message/Esp3ReceivedPacket.h"
#include "message/UTE_ReceivedMessage.h"
#include "message/DongleVersionResponseReceivedMessage.h"
#include "profiles/IType.h"
#include "ProfileHelper.h"
#include "PairingHelper.h"
#include "IMessageHandler.h"
#include <shared/communication/AsyncPortConnectionNotification.h>
#include "message/UTE_AnswerSendMessage.h"


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
   /// \brief	                     Update (only add new) device keywords if list was changed
   /// \note                        Useful in case of first run of a new plugin version
   //--------------------------------------------------------------
   void createNewKeywords(const std::string& deviceName,
                       const boost::shared_ptr<IType>& loadedDevice) const;

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
   /// \param[in] notification      The connection notification
   //--------------------------------------------------------------
   void processUnConnectionEvent(boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification> notification = boost::shared_ptr<shared::communication::CAsyncPortConnectionNotification>());

   //--------------------------------------------------------------
   /// \brief	                     Called when device was removed
   /// \param[in] deviceId          ID of the removed device
   //--------------------------------------------------------------
   void processDeviceRemoved(const std::string& deviceId);

   //--------------------------------------------------------------
   /// \brief	                     Reconfigure a device (configuration must be set in Yadoms first)
   /// \param [in] deviceId         The device ID
   /// \param [in] configuration    The device configuration
   //--------------------------------------------------------------
   void processDeviceConfiguration(const std::string& deviceId,
                                   const shared::CDataContainerSharedPtr& configuration);

   //--------------------------------------------------------------
   /// \brief	                     Called when the data are received from the UPS
   /// \param [in] message          Message received
   //--------------------------------------------------------------
   void processDataReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> message);

   //--------------------------------------------------------------
   /// \brief	                     Add signal power to historizable keyword list
   /// \param [in/out] keywords     Keyword list
   /// \param [in] deviceId         Device ID
   /// \param [in] signalPower      The current signal strength value (%)
   //--------------------------------------------------------------
   void addSignalPower(std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords,
                       const std::string& deviceId,
                       int signalPower) const;

   //--------------------------------------------------------------
   /// \brief	                     Convert dBm value to signal power
   /// \param [in] dBm              Signal power as dBm
   /// \return                      Signal power value (%)
   //--------------------------------------------------------------
   static int dbmToSignalPower(int dBm);

   //--------------------------------------------------------------
   /// \brief	                     Process received messages
   /// \param [in] esp3Packet       Message received
   //--------------------------------------------------------------
   void processRadioErp1(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);
   static void processResponse(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);
   void processDongleVersionResponse(message::CResponseReceivedMessage::EReturnCode returnCode,
                                     const message::CDongleVersionResponseReceivedMessage& dongleVersionResponse);
   void processEepTeachInMessage(boost::dynamic_bitset<> erp1UserData,
                                 boost::shared_ptr<IRorg> rorg,
                                 std::string deviceId);
   void processNoEepTeachInMessage(boost::shared_ptr<IRorg> rorg,
                                   std::string deviceId);
   void processDataTelegram(message::CRadioErp1ReceivedMessage erp1Message,
                            boost::dynamic_bitset<> erp1UserData,
                            const boost::dynamic_bitset<> erp1Status,
                            std::string deviceId);
   static void processEvent(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);
   void processUTE(message::CRadioErp1ReceivedMessage& erp1Message);
   bool sendUTEAnswer(message::CUTE_AnswerSendMessage::EResponse response,
                      boost::shared_ptr<const message::CUTE_ReceivedMessage> uteMessage,
                      bool isReversed,
                      const std::string& deviceId);

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
   /// \brief	                           Remove a device
   /// \param [in] deviceId               Device ID
   //--------------------------------------------------------------
   void removeDevice(const std::string& deviceId);

   //--------------------------------------------------------------
   /// \brief	                     Declare a device when ignoring profile
   /// \param [in] deviceId         Device ID to declare
   //--------------------------------------------------------------
   void declareDeviceWithoutProfile(const std::string& deviceId) const;

   //--------------------------------------------------------------
   /// \brief	                     Requests to EnOcean
   //--------------------------------------------------------------
   void requestDongleVersion();

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

   //--------------------------------------------------------------
   /// \brief	                     Process pairing devices
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] extraQuery       Extra query
   //--------------------------------------------------------------
   void startManualPairing(boost::shared_ptr<yApi::IYPluginApi> api,
                           boost::shared_ptr<yApi::IExtraQuery> extraQuery);

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

   //--------------------------------------------------------------
   /// \brief  The send ID (ID of EnOcean chip on the USB dongle)
   //--------------------------------------------------------------
   std::string m_senderId;

   //--------------------------------------------------------------
   /// \brief  The signal power keyword, used for each received message
   //--------------------------------------------------------------
   boost::shared_ptr<shared::plugin::yPluginApi::historization::CSignalPower> m_signalPowerKeyword;

   //--------------------------------------------------------------
   /// \brief  The pairing helper
   //--------------------------------------------------------------
   boost::shared_ptr<CPairingHelper> m_pairingHelper;
   boost::shared_ptr<shared::event::CEventTimer> m_progressPairingTimer;
};
