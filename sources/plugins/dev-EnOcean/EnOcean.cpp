#include "stdafx.h"
#include "EnOcean.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "EnOceanFactory.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"
#include "Manufacturers.h"

IMPLEMENT_PLUGIN(CEnOcean)


// Event IDs
enum
{
   //TODO faire le ménage dans les enum
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

   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   std::cout << "CEnOcean is starting..." << std::endl;

   // Load configuration values (provided by database)
   m_configuration.initializeWith(api->getConfiguration());

   // Create the connection
   createConnection(api);

   // the main loop
   while (true)
   {
      // Wait for an event
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYPluginApi::kEventStopRequested:
         {
            std::cout << "Stop requested" << std::endl;
            api->setPluginState(yApi::historization::EPluginState::kStopped);
            return;
         }
      case yApi::IYPluginApi::kEventDeviceCommand:
         {
            // Command received from Yadoms
            auto command(api->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >());
            std::cout << "Command received : " << yApi::IDeviceCommand::toString(command) << std::endl;

            onCommand(api, command);

            break;
         }
      case yApi::IYPluginApi::kEventUpdateConfiguration:
         {
            // Configuration was updated
            api->setPluginState(yApi::historization::EPluginState::kCustom, "updateConfiguration");
            auto newConfigurationData = api->getEventHandler().getEventData<shared::CDataContainer>();
            std::cout << "Update configuration..." << std::endl;
            BOOST_ASSERT(!newConfigurationData.empty()); // newConfigurationData shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

            // Close connection
            CEnOceanConfiguration newConfiguration;
            newConfiguration.initializeWith(newConfigurationData);

            // If port has changed, destroy and recreate connection (if any)
            auto needToReconnect = !connectionsAreEqual(m_configuration, newConfiguration) && !!m_port;

            if (needToReconnect)
               destroyConnection();

            // Update configuration
            m_configuration.initializeWith(newConfigurationData);

            if (needToReconnect)
               createConnection(api);
            else
               api->setPluginState(yApi::historization::EPluginState::kRunning);

            break;
         }
      case kEvtPortConnection:
         {
            if (api->getEventHandler().getEventData<bool>())
               processConnectionEvent(api);
            else
               processUnConnectionEvent(api);

            break;
         }
      case kEvtPortDataReceived:
         {
            const auto message(api->getEventHandler().getEventData<const message::CReceivedMessage>());
            processDataReceived(api, message);
            break;
         }
      case kAnswerTimeout:
         {
            std::cout << "No answer received from EnOcean dongle (timeout)" << std::endl;
            protocolErrorProcess(api);
            break;
         }
      case kProtocolErrorRetryTimer:
         {
            createConnection(api);
            break;
         }
      default:
         {
            std::cerr << "Unknown message id" << std::endl;
            break;
         }
      }
   }
}

void CEnOcean::createConnection(boost::shared_ptr<yApi::IYPluginApi> api)
{
   api->setPluginState(yApi::historization::EPluginState::kCustom, "connecting");

   // Create the port instance
   m_port = CEnOceanFactory::constructPort(m_configuration,
                                           api->getEventHandler(),
                                           kEvtPortConnection,
                                           kEvtPortDataReceived);
   m_port->start();
}

void CEnOcean::destroyConnection()
{
   m_port.reset();
}

bool CEnOcean::connectionsAreEqual(const CEnOceanConfiguration& conf1,
                                   const CEnOceanConfiguration& conf2)
{
   return (conf1.getSerialPort() == conf2.getSerialPort());
}

void CEnOcean::onCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                         boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) const
{
   if (!m_port)
      std::cout << "Command not + (dongle is not ready) : " << command << std::endl;

   //TODO
}


void CEnOcean::send(const message::CSendMessage& sendMessage) const
{
   if (!m_port)
   {
      std::cerr << "Send message failed : dongle is not ready" << std::endl;
      return;
   }

   m_port->send(sendMessage.buildBuffer());
}

void CEnOcean::send(const message::CSendMessage& sendMessage,
                    boost::function<bool(const message::CReceivedMessage& rcvMessage)> checkExpectedMessageFunction,
                    boost::function<void(const message::CReceivedMessage& rcvMessage)> onReceiveFct)
{
   shared::event::CEventHandler receivedEvtHandler;

   {
      boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
      m_onReceiveHook = [&](const message::CReceivedMessage& receivedMsg)-> bool
         {
            if (!checkExpectedMessageFunction(receivedMsg))
               return false;

            receivedEvtHandler.postEvent<const message::CReceivedMessage>(shared::event::kUserFirstId, receivedMsg);
            return true;
         };
   }

   send(sendMessage);

   if (receivedEvtHandler.waitForEvents(message::EnOceanAnswerTimeout) == shared::event::kTimeout)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
      m_onReceiveHook.clear();
      throw CProtocolException((boost::format("Asnwer timeout. Request was %1%") % sendMessage.packetType()).str());
   }

   onReceiveFct(receivedEvtHandler.getEventData<const message::CReceivedMessage>());
}

void CEnOcean::processConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "EnOcean port opened" << std::endl;

   try
   {
      requestDongleVersion(api);
      //TODO
   }
   catch (CProtocolException& e)
   {
      std::cerr << "Protocol error : " << e.what();
      protocolErrorProcess(api);
   }
   catch (shared::communication::CPortException& e)
   {
      std::cerr << "Error connecting to EnOcean dongle : " << e.what();
      // Disconnection will be notified, we just have to wait...
   }
}

void CEnOcean::protocolErrorProcess(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Retry full connection
   processUnConnectionEvent(api);
   api->getEventHandler().createTimer(kProtocolErrorRetryTimer, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(30));
}

void CEnOcean::processUnConnectionEvent(boost::shared_ptr<yApi::IYPluginApi> api)
{
   std::cout << "EnOcean connection was lost" << std::endl;
   api->setPluginState(yApi::historization::EPluginState::kError, "connectionFailed");

   destroyConnection();
}

void CEnOcean::processDataReceived(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const message::CReceivedMessage& message)
{
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
      if (m_onReceiveHook && m_onReceiveHook(message))
      {
         m_onReceiveHook.clear();
         return;
      }
   }

   try
   {
      switch (message.header().packetType())
      {
      case message::RADIO_ERP1:
         processRadioErp1(api, message);
         break;//TODO
      case message::RESPONSE: break;//TODO
      case message::RADIO_SUB_TEL: break;//TODO
      case message::EVENT:
         processEvent(api, message);
         break;//TODO
      case message::COMMON_COMMAND: break;//TODO
      case message::SMART_ACK_COMMAND: break;//TODO
      case message::REMOTE_MAN_COMMAND: break;//TODO
      case message::RADIO_MESSAGE: break;//TODO
      case message::RADIO_ERP2: break;//TODO
      case message::RADIO_802_15_4: break;//TODO
      case message::COMMAND_2_4: break;//TODO
      case message::MESSAGES: break;//TODO
      default:
         throw CProtocolException((boost::format("Unknown packet type %1%") % message.header().packetType()).str());
      }
   }
   catch (CProtocolException& e)
   {
      std::cerr << "Error processing received message : " << e.what() << std::endl;
   }
   catch (std::exception& e)
   {
      std::cerr << "Error processing received message : " << e.what() << std::endl;
   }
}


void CEnOcean::processRadioErp1(boost::shared_ptr<yApi::IYPluginApi> api,
                                const message::CReceivedMessage& message)
{
   enum ERORG
   {
      kRBS = 0xF6,
      k1BS = 0xD5,
      k4BS = 0xA5,
      kVLD = 0xD2,
      kMSC = 0xD1,
      kADT = 0xA6,
      kSM_LRN_REQ = 0xC6,
      kSM_LRN_ANS = 0xC7,
      kSM_REC = 0xA7,
      kSYS_EX = 0xC5,
      kSEC = 0x30,
      kSEC_ENCAPS = 0x31
   };

   auto rorg = static_cast<ERORG>(message.data()[0]);

   switch (rorg)
   {
   case kRBS: break;//TODO
   case k1BS: processRadioErp1_1BS(api, message.data()); break;
   case k4BS: processRadioErp1_4BS(api, message.data()); break;
   case kVLD: break;//TODO
   case kMSC: break;//TODO
   case kADT: break;//TODO
   case kSM_LRN_REQ: break;//TODO
   case kSM_LRN_ANS: break;//TODO
   case kSM_REC: break;//TODO
   case kSYS_EX: break;//TODO
   case kSEC: break;//TODO
   case kSEC_ENCAPS: break;//TODO
   default: throw CProtocolException((boost::format("Unknown RORG %1%") % rorg).str());
   }


   struct Optional
   {
      explicit Optional(const std::vector<unsigned char>& optional):
         m_subTelNum(optional[0]),
         m_destinationId(optional[1] << 24 | optional[2] << 16 | optional[3] << 8 | optional[4]),
         m_dBm(optional[5]),
         m_securityLevel(optional[6])
      {
      }

      unsigned char m_subTelNum;
      unsigned int m_destinationId;
      unsigned char m_dBm;
      unsigned char m_securityLevel;
   };

   if (message.header().optionalLength() != 7)
      throw CProtocolException((boost::format("RadioERP1 message : wrong optional size (%1%, expected 7)") % message.header().optionalLength()).str());

   Optional optional(message.optional());

   //TODO
}

void CEnOcean::processRadioErp1_1BS(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::vector<unsigned char>& data)
{
   //TODO
}

void CEnOcean::processRadioErp1_4BS(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::vector<unsigned char>& data)
{
   auto temperature = data[3];
   auto isTeachIn = !(data[4] & 0x08);

   auto senderId = extractSenderId(data, 5);

   auto status = data[9]; //TODO : status is not used...

   static const std::string keywordName("temperature");
   auto keyword(boost::make_shared<yApi::historization::CTemperature>(keywordName));

   if (isTeachIn && !m_api->deviceExists(senderId))
   {
      auto eepIsProvided = data[4] & 0x80;

      if (eepIsProvided)
      {
         //TODO
         //unsigned int manufacturer = ((DB_2 & 7) << 8) | DB_1;
         //unsigned int type = ((DB_3 & 3) << 5) | (DB_2 >> 3);
         //unsigned int func = DB_3 >> 2;
      }
      else
      {
         unsigned int manufacturer = CManufacturers::kMulti_user_Manufacturer_ID;
         unsigned int type = 5;//TODO ça serait mieux avec une constante
         unsigned int func = 2;//TODO ça serait mieux avec une constante
      }

      m_api->declareDevice(senderId, std::string(), keyword);
   }

   keyword->set(scaleToDouble(temperature, 255, 0, -40, 0));
   m_api->historizeData(senderId, keyword);
}

double CEnOcean::scaleToDouble(int inValue,
                               int inRangeMin,
                               int inRangeMax,
                               int outScaleMin,
                               int outScaleMax)
{
   double inRange = inRangeMax - inRangeMin;
   double outScale = outScaleMax - outScaleMin;

   if (inRange == 0 || outScale == 0)
      throw std::out_of_range((boost::format("Unable to scale value %1% (min=%2%, max=%3%) to range [%4%, %5%]") % inValue % inRangeMin % inRangeMax % outScaleMin % outScaleMax).str());

   return (inValue - inRangeMin) * (outScale / inRange) + outScaleMin;
}

std::string CEnOcean::extractSenderId(const std::vector<unsigned char>& data,
                                      int startIndex)
{
   return std::to_string(
      (data[startIndex + 3] << 24) +
      (data[startIndex + 2] << 16) +
      (data[startIndex + 1] << 8) +
      (data[startIndex]));
}

void CEnOcean::processEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                            const message::CReceivedMessage& message)
{
   if (message.header().dataLength() < 1)
      throw CProtocolException((boost::format("RadioERP1 message : wrong data size (%1%, < 1)") % message.header().dataLength()).str());

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

   auto eventCode = message.data()[0];

   switch (eventCode)
   {
   case SA_RECLAIM_NOT_SUCCESSFUL: break; //TODO
   case SA_CONFIRM_LEARN: break; //TODO
   case SA_LEARN_ACK: break; //TODO
   case CO_READY: break; //TODO
   case CO_EVENT_SECUREDEVICES: break; //TODO
   case CO_DUTYCYCLE_LIMIT: break; //TODO
   case CO_TRANSMIT_FAILED: break; //TODO
   default:
      throw CProtocolException((boost::format("Unknown event code %1%") % eventCode).str());
   }

   //TODO
}

void CEnOcean::requestDongleVersion(boost::shared_ptr<yApi::IYPluginApi> api)
{
   message::CCommandSendMessage sendMessage;
   sendMessage.appendData({ message::CO_RD_VERSION });

   send(sendMessage,
        [](const message::CReceivedMessage& rcvMessage)
        {
           return rcvMessage.header().packetType() == message::RESPONSE;
        },
        [](const message::CReceivedMessage& rcvMessage)
        {
           if (rcvMessage.header().packetType() != message::RESPONSE)
              throw CProtocolException((boost::format("Invalid packet type %1%, expected 2(RESPONSE). Request was CO_RD_VERSION.") % rcvMessage.header().packetType()).str());

           if (rcvMessage.header().dataLength() != 33)
              throw CProtocolException((boost::format("Invalid data length %1%, expected 33. Request was CO_RD_VERSION.") % rcvMessage.header().dataLength()).str());

           auto returnCode = static_cast<message::EReturnCode>(rcvMessage.data()[0]);

           if (returnCode == message::RET_NOT_SUPPORTED)
           {
              std::cout << "CO_RD_VERSION request returned not supported" << std::endl;
              return;
           }

           if (returnCode != message::RET_OK)
              throw CProtocolException((boost::format("Unexpected return code %1%. Request was CO_RD_VERSION.") % returnCode).str());

           struct Version
           {
              unsigned int m_main;
              unsigned int m_beta;
              unsigned int m_alpha;
              unsigned int m_build;

              std::string toString() const
              {
                 std::ostringstream str;
                 str << m_main << '.' << m_beta << '.' << m_alpha << '.' << m_build;
                 return str.str();
              }
           };
           Version appVersion;
           appVersion.m_main = rcvMessage.data()[1];
           appVersion.m_beta = rcvMessage.data()[2];
           appVersion.m_alpha = rcvMessage.data()[3];
           appVersion.m_build = rcvMessage.data()[4];

           Version apiVersion;
           apiVersion.m_main = rcvMessage.data()[5];
           apiVersion.m_beta = rcvMessage.data()[6];
           apiVersion.m_alpha = rcvMessage.data()[7];
           apiVersion.m_build = rcvMessage.data()[8];

           auto chipId = rcvMessage.data()[9] << 24 | rcvMessage.data()[10] << 16 | rcvMessage.data()[11] << 8 | rcvMessage.data()[12];

           auto chipVersion = rcvMessage.data()[13] << 24 | rcvMessage.data()[14] << 16 | rcvMessage.data()[15] << 8 | rcvMessage.data()[16];

           std::string appDescription(rcvMessage.data().begin() + 17, rcvMessage.data().end());

           std::cout << "EnOcean dongle Version " << appVersion.toString() <<
              "\"" << appDescription << "\"" <<
              ", api " << apiVersion.toString() <<
              ", chipId " << chipId <<
              ", chipVersion " << chipVersion <<
              std::endl;
        });
}

