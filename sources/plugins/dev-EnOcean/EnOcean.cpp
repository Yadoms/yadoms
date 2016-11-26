#include "stdafx.h"
#include "EnOcean.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "Factory.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"
#include "profiles/generated-manufacturers.h"
#include "4BSTeachinVariant2.h"
#include "profiles/bitsetHelpers.hpp"
#include <shared/exception/EmptyResult.hpp>
#include "ProfileHelper.h"
#include "message/CommonCommandSendMessage.h"
#include "message/UTE_AnswerSendMessage.h"
#include "message/ResponseReceivedMessage.h"
#include "DeviceConfigurationHelper.h"


IMPLEMENT_PLUGIN(CEnOcean)


// Event IDs
enum
{
   kEvtPortConnection = yApi::IYPluginApi::kPluginFirstEventId, // Always start from yApi::IYPluginApi::kPluginFirstEventId
   kEvtPortDataReceived,
   kProtocolErrorRetryTimer,
   kAnswerTimeout,
};


CEnOcean::CEnOcean()
{
}

CEnOcean::~CEnOcean()
{
}

void CEnOcean::doWork(boost::shared_ptr<yApi::IYPluginApi> api)
{
   m_api = api;

   m_api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   std::cout << "EnOcean is starting..." << std::endl;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(m_api->getConfiguration());

   // Load known devices
   loadAllDevices();

   // Create the connection
   createConnection();

   // the main loop
   while (true)
   {
      try
      {
         // Wait for an event
         switch (m_api->getEventHandler().waitForEvents())
         {
         case yApi::IYPluginApi::kEventStopRequested:
            {
               std::cout << "Stop requested" << std::endl;
               m_api->setPluginState(yApi::historization::EPluginState::kStopped);
               return;
            }

         case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               m_api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
               auto newConfigurationData = m_api->getEventHandler().getEventData<shared::CDataContainer>();
               std::cout << "Update configuration..." << std::endl;
               BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Close connection
               CConfiguration newConfiguration;
               newConfiguration.initializeWith(newConfigurationData);

               // If port has changed, destroy and recreate connection (if any)
               auto needToReconnect = !connectionsAreEqual(m_configuration, newConfiguration) && !!m_port;

               if (needToReconnect)
                  destroyConnection();

               // Update configuration
               m_configuration.initializeWith(newConfigurationData);

               if (needToReconnect)
                  createConnection();
               else
                  m_api->setPluginState(yApi::historization::EPluginState::kRunning);

               break;
            }

         case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command received from Yadoms
               auto command(m_api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand>>());
               std::cout << "Command received : " << yApi::IDeviceCommand::toString(command) << std::endl;

               processDeviceCommand(command);

               break;
            }

         case yApi::IYPluginApi::kEventDeviceRemoved:
            {
               auto device = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceRemoved>>();
               std::cout << device->device() << " was removed" << std::endl;
               processDeviceRemmoved(device);
               break;
            }

         case yApi::IYPluginApi::kSetDeviceConfiguration:
            {
               // Yadoms sent the new device configuration. Plugin must apply this configuration to device.
               auto deviceConfiguration = api->getEventHandler().getEventData<boost::shared_ptr<const yApi::ISetDeviceConfiguration>>();
               processDeviceConfiguration(deviceConfiguration->device(),
                                          deviceConfiguration->configuration());
               break;
            }

         case kEvtPortConnection:
            {
               if (m_api->getEventHandler().getEventData<bool>())
                  processConnectionEvent();
               else
                  processUnConnectionEvent();

               break;
            }
         case kEvtPortDataReceived:
            {
               const auto message(m_api->getEventHandler().getEventData<boost::shared_ptr<const message::CEsp3ReceivedPacket>>());
               processDataReceived(message);
               break;
            }
         case kAnswerTimeout:
            {
               std::cerr << "No answer received from EnOcean dongle (timeout)" << std::endl;
               protocolErrorProcess();
               break;
            }
         case kProtocolErrorRetryTimer:
            {
               createConnection();
               break;
            }
         default:
            {
               std::cerr << "Unknown message id" << std::endl;
               break;
            }
         }
      }
      catch (std::logic_error& e)
      {
         std::cerr << "Logical error : " << e.what() << std::endl;
      }
      catch (CProtocolException& e)
      {
         std::cerr << "Error communicationg with EnOcean dongle " << e.what() << std::endl;
         protocolErrorProcess();
      }
   }
}

void CEnOcean::loadAllDevices()
{
   auto devices = m_api->getAllDevices();
   for (const auto& deviceId : devices)
   {
      try
      {
         auto deviceConfiguration = m_api->getDeviceConfiguration(deviceId);
         if (deviceId == "pluginState" || deviceConfiguration.empty())
            continue; // Not configured device

         CProfileHelper profileHelper(deviceConfiguration.get<std::string>("profile.activeSection"));
         auto device = createDevice(deviceId,
                                    profileHelper);

         m_devices[deviceId] = device;
      }
      catch (shared::exception::CEmptyResult&)
      {
         // Don't add a not configured device to the m_devices list
      }
      catch (std::exception& e)
      {
         // Don't add a wrong configured device to the m_devices list
         std::cerr << "Error loading device from database : device " << deviceId << " is malformed (" << e.what() << "), will be ignored" << std::endl;
      }
   }
}

void CEnOcean::createConnection()
{
   m_api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   // Create the port instance
   m_port = CFactory::constructPort(m_configuration);

   m_messageHandler = CFactory::constructMessageHandler(m_port,
                                                        m_api->getEventHandler(),
                                                        kEvtPortDataReceived);

   m_port->subscribeForConnectionEvents(m_api->getEventHandler(),
                                        kEvtPortConnection);

   m_port->setReceiveBufferHandler(CFactory::constructReceiveBufferHandler(m_messageHandler));

   m_port->start();
}

void CEnOcean::destroyConnection()
{
   m_port.reset();
}

bool CEnOcean::connectionsAreEqual(const CConfiguration& conf1,
                                   const CConfiguration& conf2)
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}

boost::shared_ptr<IType> CEnOcean::createDevice(const std::string& deviceId,
                                                const CProfileHelper& profileHelper) const
{
   return CRorgs::createRorg(profileHelper.rorg())->createFunc(profileHelper.func())->createType(profileHelper.type(),
                                                                                                 deviceId,
                                                                                                 m_api);
}

std::string CEnOcean::generateModel(const std::string& model,
                                    const std::string& manufacturer,
                                    const CProfileHelper& profile) const
{
   if (!model.empty())
      return model;

   std::string generatedModel;

   if (!manufacturer.empty())
      generatedModel += manufacturer + std::string(" - ");

   generatedModel += createDevice(std::string(), profile)->title();

   return generatedModel;
}

void CEnOcean::processDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command)
{
   if (!m_port)
   {
      std::cout << "Unable to process command : dongle is not ready" << std::endl;
      return;
   }

   if (m_devices.find(command->getDevice()) == m_devices.end())
   {
      std::cerr << "Unable to process command : device " << command->getDevice() << " unknown" << std::endl;
      return;
   }

   m_devices[command->getDevice()]->sendCommand(command->getKeyword(),
                                                command->getBody(),
                                                m_messageHandler);
}

void CEnOcean::processConnectionEvent()
{
   std::cout << "EnOcean port opened" << std::endl;

   try
   {
      requestDongleVersion();
   }
   catch (CProtocolException& e)
   {
      std::cerr << "Protocol error : " << e.what();
      protocolErrorProcess();
   }
   catch (shared::communication::CPortException& e)
   {
      std::cerr << "Error connecting to EnOcean dongle : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CEnOcean::protocolErrorProcess()
{
   // Retry full connection
   processUnConnectionEvent();
   m_api->getEventHandler().createTimer(kProtocolErrorRetryTimer,
                                        shared::event::CEventTimer::kOneShot
                                        , boost::posix_time::seconds(30));
}

void CEnOcean::processUnConnectionEvent()
{
   std::cout << "EnOcean connection was lost" << std::endl;
   m_api->setPluginState(yApi::historization::EPluginState::kError, "connectionFailed");

   destroyConnection();
}

void CEnOcean::processDeviceRemmoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> deviceRemoved)
{
   if (m_devices.find(deviceRemoved->device()) == m_devices.end())
      return;

   m_devices.erase(deviceRemoved->device());
}

void CEnOcean::processDeviceConfiguration(const std::string& deviceId,
                                          const shared::CDataContainer& configuration)
{
   try
   {
      auto selectedProfile = CProfileHelper(configuration.get<std::string>("profile.activeSection"));
      auto manufacturer = configuration.get<std::string>("manufacturer");

      std::cout << "Device \"" << deviceId << "\" is configurated as " << selectedProfile.profile() << std::endl;

      if (m_devices.find(deviceId) == m_devices.end() || m_devices[deviceId]->profile() != selectedProfile.profile())
      {
         // Device was not configured or profile changed, recreate local device and all keywords
         if (m_devices.find(deviceId) != m_devices.end())
            for (const auto& keyword : m_api->getAllKeywords(deviceId))
               m_api->removeKeyword(deviceId, keyword);

         // Don't recreate device in Yadoms, unless it will change of ID
         auto device = createDevice(deviceId,
                                    selectedProfile);
         m_api->declareKeywords(deviceId,
                                device->allHistorizers());
         m_devices[deviceId] = device;
      }

      // Send configuration to device
      try
      {
         m_devices[deviceId]->sendConfiguration(configuration.get<shared::CDataContainer>("profile.content." + m_devices[deviceId]->profile() + ".content"),
                                                m_messageHandler);
      }
      catch (std::exception& e)
      {
         throw std::runtime_error(std::string("Unable to configure device : ") + e.what());
      }
   }
   catch (shared::exception::CEmptyResult&)
   {
      std::cerr << "Unable to configure device : unknown device \"" << deviceId << "\"" << std::endl;
   }
   catch (std::exception& e)
   {
      std::cerr << "Unable to configure device : " << e.what() << std::endl;
   }
}

void CEnOcean::processDataReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> message)
{
   try
   {
      switch (message->header().packetType())
      {
      case message::RADIO_ERP1:
         processRadioErp1(message);
         break;
      case message::RESPONSE:
         processResponse(message);
         break;
      case message::EVENT:
         processEvent(message);
         break;
      default:
         throw CProtocolException((boost::format("Unknown or unsupported received packet type %1%") % message->header().packetType()).str());
      }
   }
   catch (CProtocolException& e)
   {
      std::cerr << "Error processing received message : " << e.what() << std::endl;
   }
   catch (std::exception& e)
   {
      std::cerr << "Error processing received message : " << e.what() << std::endl;
      std::cerr << "Message was : " << message->dump() << std::endl;
   }
}


void CEnOcean::processRadioErp1(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
{
   message::CRadioErp1ReceivedMessage erp1Message(esp3Packet);

   if (erp1Message.rorg() == CRorgs::kUTE_Telegram)
   {
      processUTE(message::CUTE_ReceivedMessage(erp1Message));
      return;
   }

   // Create associated RORG object
   auto erp1UserData = bitset_from_bytes(erp1Message.userData());
   auto erp1Status = bitset_from_byte(erp1Message.status());
   auto rorg = CRorgs::createRorg(erp1Message.rorg());
   auto deviceId = erp1Message.senderId();

   if (rorg->isTeachIn(erp1UserData))
   {
      // Teachin telegram

      if (!rorg->isEepProvided(erp1UserData))
      {
         if (m_api->deviceExists(deviceId))
         {
            // Device exist.
            // It is ether configured or not, but this teachin message give us nothing new (no EEP is provided), so ignore it
            std::cout << "Device " << deviceId << " already declared, teachin message ignored." << std::endl;
            return;
         }

         declareDeviceWithoutProfile(deviceId);
         return;
      }

      if (rorg->id() != CRorgs::k4BS_Telegram)
         throw std::domain_error("Teach-in telegram is only supported for 4BS telegram for now. Please report to Yadoms-team.");

      // Special-case of 4BS teachin mode Variant 2 (profile is provided in the telegram)
      C4BSTeachinVariant2 teachInData(erp1UserData);

      try
      {
         auto profile = CProfileHelper(rorg->id(),
                                       teachInData.funcId(),
                                       teachInData.typeId());
         auto manufacturerName = CManufacturers::name(teachInData.manufacturerId());

         CDeviceConfigurationHelper deviceConfiguration(profile,
                                                        manufacturerName);

         if (m_api->deviceExists(deviceId))
         {
            // Device already exist, just reconfigure it
            m_api->updateDeviceConfiguration(deviceId,
                                             deviceConfiguration.configuration());

            processDeviceConfiguration(deviceId,
                                       deviceConfiguration.configuration());

            return;
         }

         // Device not exist, declare it
         declareDevice(deviceId,
                       profile,
                       manufacturerName);

         m_api->updateDeviceConfiguration(deviceId,
                                          deviceConfiguration.configuration());
      }
      catch (std::exception& e)
      {
         std::cerr << "Unable to declare device : " << e.what() << std::endl;
      }
   }
   else
   {
      // Data telegram

      // Declare device if unknown
      if (m_devices.find(deviceId) == m_devices.end())
      {
         if (m_api->deviceExists(deviceId))
         {
            std::cout << "Device " << deviceId << " already declared but not configured, message ignored." << std::endl;
            return;
         }

         declareDeviceWithoutProfile(deviceId);
         return;
      }

      auto device = m_devices[deviceId];

      auto keywordsToHistorize = device->states(static_cast<unsigned char>(erp1Message.rorg()),
                                                erp1UserData,
                                                erp1Status);
      if (keywordsToHistorize.empty())
      {
         std::cout << "Received message for id#" << deviceId << ", but nothing to historize" << std::endl;
         return;
      }

      std::cout << "Received message for id#" << deviceId << " : " << std::endl;
      for (const auto& kw: keywordsToHistorize)
         std::cout << "  - " << kw->getKeyword() << " = " << kw->formatValue() << std::endl;;

      m_api->historizeData(deviceId, keywordsToHistorize);
   }
}

void CEnOcean::declareDeviceWithoutProfile(const std::string& deviceId) const
{
   std::cout << "New device declared : " << std::endl;
   std::cout << "  - Id           : " << deviceId << std::endl;
   std::cout << "  - Profile      : Unknown. No historization until user enter profile." << std::endl;

   m_api->declareDevice(deviceId,
                        std::string(),
                        std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>());
}

void CEnOcean::processResponse(boost::shared_ptr<const message::CEsp3ReceivedPacket>)
{
   std::cerr << "Unexpected response received" << std::endl;
}

void CEnOcean::processDongleVersionResponse(unsigned char returnCode,
                                            const message::CDongleVersionResponseReceivedMessage& dongleVersionResponse)
{
   if (returnCode == message::RET_NOT_SUPPORTED)
   {
      std::cout << "CO_RD_VERSION request returned not supported" << std::endl;
      return;
   }

   if (returnCode != message::RET_OK)
   {
      std::cerr << "Unexpected return code " << returnCode << ". Request was CO_RD_VERSION." << std::endl;
      return;
   }

   std::cout << dongleVersionResponse.fullVersion() << std::endl;
}

void CEnOcean::processEvent(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
{
   if (esp3Packet->header().dataLength() < 1)
      throw CProtocolException((boost::format("RadioERP1 message : wrong data size (%1%, < 1)") % esp3Packet->header().dataLength()).str());

   enum
      {
         SA_RECLAIM_NOT_SUCCESSFUL = 0x01,
         SA_CONFIRM_LEARN = 0x02,
         SA_LEARN_ACK = 0x03,
         CO_READY = 0x04,
         CO_EVENT_SECUREDEVICES = 0x05,
         CO_DUTYCYCLE_LIMIT = 0x06,
         CO_TRANSMIT_FAILED = 0x07
      };

   auto eventCode = esp3Packet->data()[0];
   std::cout << "Event " << eventCode << " received" << std::endl;
}

void CEnOcean::processUTE(const message::CUTE_ReceivedMessage& uteMessage)
{
   if (uteMessage.teachInRequest() != message::CUTE_ReceivedMessage::kTeachInRequest &&
      uteMessage.teachInRequest() != message::CUTE_ReceivedMessage::kNotSpecified)
   {
      std::cout << "UTE message : teach-in request type " << uteMessage.teachInRequest() << "not supported, message ignored" << std::endl;
      return;
   }

   if (uteMessage.command() != message::CUTE_ReceivedMessage::kTeachInQuery)
   {
      std::cout << "UTE message : command type " << uteMessage.command() << "not supported, message ignored" << std::endl;
      return;
   }

   auto deviceId = uteMessage.erp1().senderId();
   auto profile = CProfileHelper(uteMessage.rorg(), uteMessage.func(), uteMessage.type());
   auto manufacturerName = CManufacturers::name(uteMessage.manufacturerId());

   auto response = message::CUTE_AnswerSendMessage::kRequestAccepted;
   if (m_devices.find(deviceId) == m_devices.end())
   {
      try
      {
         declareDevice(deviceId,
                       profile,
                       manufacturerName,
                       generateModel(std::string(), manufacturerName, profile));

         m_api->updateDeviceConfiguration(deviceId,
                                          CDeviceConfigurationHelper(profile, manufacturerName).configuration());
      }
      catch (std::exception& e)
      {
         std::cerr << "Fail to declare device (Universal teachin) : " << e.what() << std::endl;
         response = message::CUTE_AnswerSendMessage::kRequestNotAccepted;
      }
   }

   if (uteMessage.teachInResponseExpected())
   {
      message::CUTE_AnswerSendMessage sendMessage(0,
                                                  deviceId,
                                                  uteMessage.bidirectionalCommunication(),
                                                  response,
                                                  uteMessage.channelNumber(),
                                                  uteMessage.manufacturerId(),
                                                  uteMessage.type(),
                                                  uteMessage.func(),
                                                  uteMessage.rorg());

      unsigned char returnCode;
      if (!m_messageHandler->send(sendMessage,
                                  [](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                                  {
                                     return esp3Packet->header().packetType() == message::RESPONSE;
                                  },
                                  [&](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                                  {
                                     returnCode = message::CResponseReceivedMessage(esp3Packet).returnCode();
                                  }))
         throw CProtocolException("Unable to send UTE response, timeout waiting acknowledge");

      if (returnCode != message::RET_OK)
         std::cerr << "TeachIn response not successfully acknowledged : " << returnCode << std::endl;
   }
}

boost::shared_ptr<IType> CEnOcean::declareDevice(const std::string& deviceId,
                                                 const CProfileHelper& profile,
                                                 const std::string& manufacturer,
                                                 const std::string& model)
{
   auto device = createDevice(deviceId,
                              profile);

   auto modelLabel = generateModel(model,
                                   manufacturer,
                                   profile);

   auto keywordsToDeclare = device->allHistorizers();
   if (keywordsToDeclare.empty())
   {
      std::stringstream s;
      s << "Can not declare device id#" << deviceId
         << " (" << profile.profile()
         << ") : no keyword to declare" << std::endl;
      throw std::logic_error(s.str());
   }

   if (m_devices.find(deviceId) != m_devices.end())
      throw std::logic_error("Device " + deviceId + " already exist");

   m_api->declareDevice(deviceId,
                        modelLabel,
                        keywordsToDeclare);

   std::cout << "New device declared : " << std::endl;
   std::cout << "  - Id           : " << deviceId << std::endl;
   std::cout << "  - Profile      : " << profile.profile() << std::endl;
   std::cout << "  - Manufacturer : " << manufacturer << std::endl;
   std::cout << "  - Model        : " << modelLabel << std::endl;
   std::cout << "  - RORG         : " << CRorgs::createRorg(profile.rorg())->title() << std::endl;
   std::cout << "  - FUNC         : " << CRorgs::createRorg(profile.rorg())->createFunc(profile.func())->title() << std::endl;
   std::cout << "  - TYPE         : " << device->title() << std::endl;

   m_devices[deviceId] = device;
   return device;
}

void CEnOcean::requestDongleVersion() const
{
   message::CCommonCommandSendMessage sendMessage(message::CCommonCommandSendMessage::CO_RD_VERSION);

   boost::shared_ptr<const message::CEsp3ReceivedPacket> answer;
   if (!m_messageHandler->send(sendMessage,
                               [](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                               {
                                  return esp3Packet->header().packetType() == message::RESPONSE;
                               },
                               [&](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                               {
                                  answer = esp3Packet;
                               }))
      throw CProtocolException("Unable to get Dongle Version, timeout waiting answer");

   if (answer->header().dataLength() != message::RESPONSE_DONGLE_VERSION_SIZE)
      throw CProtocolException((boost::format("Invalid data length %1%, expected 33. Request was CO_RD_VERSION.") % answer->header().dataLength()).str());

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);
   processDongleVersionResponse(response->returnCode(),
                                message::CDongleVersionResponseReceivedMessage(response));

   m_api->setPluginState(yApi::historization::EPluginState::kRunning);
}
