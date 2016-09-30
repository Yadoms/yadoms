#include "stdafx.h"
#include "EnOcean.h"
#include <plugin_cpp_api/ImplementationHelper.h>
#include "EnOceanFactory.h"
#include <shared/communication/PortException.hpp>
#include "ProtocolException.hpp"
#include "generated-manufacturers.h"

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
            const auto message(api->getEventHandler().getEventData<const message::CReceivedEsp3Packet>());
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
                    boost::function<bool(const message::CReceivedEsp3Packet& rcvMessage)> checkExpectedMessageFunction,
                    boost::function<void(const message::CReceivedEsp3Packet& rcvMessage)> onReceiveFct)
{
   shared::event::CEventHandler receivedEvtHandler;

   {
      boost::lock_guard<boost::recursive_mutex> lock(m_onReceiveHookMutex);
      m_onReceiveHook = [&](const message::CReceivedEsp3Packet& receivedMsg)-> bool
         {
            if (!checkExpectedMessageFunction(receivedMsg))
               return false;

            receivedEvtHandler.postEvent<const message::CReceivedEsp3Packet>(shared::event::kUserFirstId, receivedMsg);
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

   onReceiveFct(receivedEvtHandler.getEventData<const message::CReceivedEsp3Packet>());
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
                                   const message::CReceivedEsp3Packet& message)
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
                                const message::CReceivedEsp3Packet& esp3Packet)
{
   message::CRadioErp1Message erp1Message(esp3Packet);

   // Get device details from database
   auto device = retrieveDevice(erp1Message.senderId());

   switch (erp1Message.rorg())
   {
   case CRorgs::kRPS_Telegram: break;//TODO
   case CRorgs::k1BS_Telegram: processRadioErp1_1BS(api, esp3Packet.data()); break;
   case CRorgs::k4BS_Telegram: processRadioErp1_4BS(api, device, message::C4BSMessage(erp1Message)); break;
   case CRorgs::kVLD_Telegram: break;//TODO
   default: throw CProtocolException((boost::format("Unknown RORG %1%") % erp1Message.rorg()).str());
   }


   //TODO
   //struct Optional
   //{
   //   explicit Optional(const std::vector<unsigned char>& optional):
   //      m_subTelNum(optional[0]),
   //      m_destinationId(optional[1] << 24 | optional[2] << 16 | optional[3] << 8 | optional[4]),
   //      m_dBm(optional[5]),
   //      m_securityLevel(optional[6])
   //   {
   //   }

   //   unsigned char m_subTelNum;
   //   unsigned int m_destinationId;
   //   unsigned char m_dBm;
   //   unsigned char m_securityLevel;
   //};

   //if (message.header().optionalLength() != 7)
   //   throw CProtocolException((boost::format("RadioERP1 message : wrong optional size (%1%, expected 7)") % message.header().optionalLength()).str());

   //Optional optional(message.optional());

   //TODO
}

void CEnOcean::processRadioErp1_1BS(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::vector<unsigned char>& data)
{
   //TODO
}

void CEnOcean::processRadioErp1_4BS(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const CDevice& device,
                                    const message::C4BSMessage& data)
{
   if (data.isTeachIn())
   {
      // Teachin telegram
      if (!data.isEepProvided())
         throw std::out_of_range((boost::format("4BS teach-in telegram of variation 1 (with no profile provided) is not supported. Please report to Yadoms-team. Telegram \"%1%\"") % data.dump()).str());

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywords;
      auto model(CManufacturers::name(data.manufacturerId()));

      switch (data.func())
      {
      case 2 /*TODO mettre une constante*/:
         {
            // Temperature Sensors
            model += std::string(" - ") + "Temperature Sensors";/*TODO à tirer du XML */
            switch (device.type())
            {
            case 1 /*TODO mettre une constante*/:model += std::string(" (") + "Temperature Sensor Range -40°C to 0°C" /*TODO à tirer du XML */ + ")"; break;
            case 2 /*TODO mettre une constante*/:model += std::string(" (") + "Temperature Sensor Range -30°C to +10°C" /*TODO à tirer du XML */ + ")"; break;
            default:
               throw std::out_of_range((boost::format("Unknown TYPE value (%1%) for FUNC %2%") % device.type() % device.func()).str());
            }
            
            keywords.push_back(boost::make_shared<yApi::historization::CTemperature>("temperature"));
            break;
         }
      case 4 /*TODO mettre une constante*/:
         {
            // Temperature and Humidity Sensors
            model += std::string(" - ") + "Temperature and Humidity Sensor";/*TODO à tirer du XML */
            switch (device.type())
            {
            case 1 /*TODO mettre une constante*/: model += std::string(" (") + "Range 0°C to +40°C and 0% to 100%" /*TODO à tirer du XML */ + ")"; break;
            case 2 /*TODO mettre une constante*/: model += std::string(" (") + "Range -20°C to +60°C and 0% to 100%" /*TODO à tirer du XML */ + ")"; break;
            default:
               throw std::out_of_range((boost::format("Unknown TYPE value (%1%) for FUNC %2%") % device.type() % device.func()).str());
            }

            keywords.push_back(boost::make_shared<yApi::historization::CTemperature>("temperature"));
            keywords.push_back(boost::make_shared<yApi::historization::CHumidity>("humidity"));
            break;
         }
         default:
            throw std::out_of_range((boost::format("Unknown FUNC value (%1%)") % device.func()).str());
      }

      m_api->declareDevice(std::to_string(device.id()), model, keywords);
   }
   else
   {
      // Data telegram
      switch (device.func())
      {
      case 2 /*TODO mettre une constante*/:
      {
         // Temperature Sensors
         double temperature;
         switch (device.type())
         {
         case 1 /*TODO mettre une constante*/:
         {
            // Temperature Sensor Range -40°C to 0°C
            temperature = scaleToDouble(data.db1(),// TODO position de la valeur à récupérer de l'XML
               0, 255,// TODO valeurs à récupérer de l'XML
               -40, 0);// TODO valeurs à récupérer de l'XML
            break;
         }
         case 2 /*TODO mettre une constante*/:
         {
            // Temperature Sensor Range -30°C to +10°C
            temperature = scaleToDouble(data.db1(),// TODO position de la valeur à récupérer de l'XML
               0, 255,// TODO valeurs à récupérer de l'XML
               -30, +10);// TODO valeurs à récupérer de l'XML
            break;
         }

         default:
            throw std::out_of_range((boost::format("Unknown TYPE value (%1%) for FUNC %2%") % device.type() % device.func()).str());
         }


         static const std::string keywordName("temperature");
         auto keyword(boost::make_shared<yApi::historization::CTemperature>(keywordName));
         keyword->set(temperature);
         m_api->historizeData(std::to_string(device.id()), keyword);
         break;
      }
      default:
         throw std::out_of_range((boost::format("Unknown FUNC value (%1%)") % device.func()).str());
      }
   }
}

double CEnOcean::scaleToDouble(int rawValue,
                               int rangeMin,
                               int rangeMax,
                               int scaleMin,
                               int scaleMax)
{
   unsigned int multiplier = (scaleMax - scaleMin) / (rangeMax - rangeMin);
   return multiplier * (rawValue - rangeMin) + scaleMin;
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

CDevice CEnOcean::retrieveDevice(unsigned int deviceId)
{
   //TODO interroger la base

   return CDevice(deviceId, CManufacturers::kThermokon, 2, 1);//TODO revoir tous les paramètres
}

void CEnOcean::processEvent(boost::shared_ptr<yApi::IYPluginApi> api,
                            const message::CReceivedEsp3Packet& esp3Packet)
{
   if (esp3Packet.header().dataLength() < 1)
      throw CProtocolException((boost::format("RadioERP1 message : wrong data size (%1%, < 1)") % esp3Packet.header().dataLength()).str());

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

   auto eventCode = esp3Packet.data()[0];

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
        [](const message::CReceivedEsp3Packet& rcvMessage)
        {
           return rcvMessage.header().packetType() == message::RESPONSE;
        },
        [](const message::CReceivedEsp3Packet& rcvMessage)
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

