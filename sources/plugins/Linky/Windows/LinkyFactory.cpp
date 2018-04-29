#include "stdafx.h"
#include "LinkyFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "../LinkyDecoder.h"
#include "../HistoricDecoder.h"
#include <shared/Log.h>
#include "FT2xxSerialPort.h"

const int CLinkyFactory::m_baudRateStandard = 9600;
const int CLinkyFactory::m_baudRateHistoric = 1200;

CLinkyFactory::~CLinkyFactory()
{}

boost::shared_ptr<shared::communication::IAsyncPort> CLinkyFactory::constructPort(const EProtocolType type,
                                                                                  const ILinkyConfiguration& configuration,
                                                                                  shared::event::CEventHandler& eventHandler,
                                                                                  boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler,
                                                                                  int evtPortConnectionId)
{
   int baudRate = m_baudRateStandard;
   YADOMS_LOG(information) << "Connecting Linky on serial port " << configuration.getSerialPort() << "..." ;

   boost::shared_ptr<shared::communication::IAsyncPort> port = boost::make_shared<shared::communication::CFT2xxSerialPort>(configuration.getSerialPort(),
                                                                                                                           boost::asio::serial_port_base::baud_rate(1200),
                                                                                                                           boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even),
                                                                                                                           boost::asio::serial_port_base::character_size(7),
                                                                                                                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                                                                                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

   auto FTDIPortList = boost::static_pointer_cast<shared::communication::CFT2xxSerialPort>(port)->getPortComNumber();
   bool isFTDISerialPort = false;
   boost::regex reg("\\d+");
   boost::smatch match;

   //Set parameters
   if (boost::regex_search(configuration.getSerialPort(), match, reg))
   {
      std::vector<int>::const_iterator iterator;
      int counter = 0;

      // change all hardware names
      for (iterator = FTDIPortList.begin(); iterator != FTDIPortList.end(); ++iterator)
      {
         auto COMPort = std::string(match[0].first, match[0].second);

         if (match.size() != 0 && (COMPort == boost::lexical_cast<std::string>((*iterator))))
         {
            YADOMS_LOG(information) << "The serial port is a FTDI port. The FTDI driver will be used instead of the generic serial driver.";

            // Set the port number
            boost::static_pointer_cast<shared::communication::CFT2xxSerialPort>(port)->setPortNumber(counter);
            isFTDISerialPort = true;
            break;
         }
         ++counter;
      }
   }

   if (type == Standard)
      baudRate = m_baudRateStandard;
   else
      baudRate = m_baudRateHistoric;

   if (!isFTDISerialPort)
   {
      port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
                                                                         boost::asio::serial_port_base::baud_rate(baudRate),
                                                                         boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even),
                                                                         boost::asio::serial_port_base::character_size(7),
                                                                         boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                                         boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
   }

   port->subscribeForConnectionEvents(eventHandler,
                                      evtPortConnectionId);

   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}

boost::shared_ptr<CLinkyReceiveBufferHandler> CLinkyFactory::GetBufferHandler(const EProtocolType type, 
                                                                              shared::event::CEventHandler& eventHandler,
                                                                              int evtPortDataReceived,
                                                                              const bool isDeveloperMode)
{
   boost::shared_ptr<shared::communication::IBufferLogger> logger;

   logger = boost::make_shared<shared::communication::CAsciiBufferLogger>("trace");

   return boost::make_shared<CLinkyReceiveBufferHandler>(type,
                                                         eventHandler,
                                                         evtPortDataReceived,
                                                         logger,
                                                         isDeveloperMode);
}

boost::shared_ptr<IDecoder> CLinkyFactory::constructDecoder(const EProtocolType type, 
                                                            boost::shared_ptr<yApi::IYPluginApi> api)
{
   if (type == Standard)
      return boost::make_shared<CLinkyDecoder>(api);
   else
      return boost::make_shared<CHistoricDecoder>(api);
}

void CLinkyFactory::FTDI_ActivateGPIO(boost::shared_ptr<shared::communication::IAsyncPort> serialPort, int channel)
{
   auto port = boost::dynamic_pointer_cast<shared::communication::CFT2xxSerialPort>(serialPort);

   if (port)
      port->activateGPIO(channel);
}

void CLinkyFactory::FTDI_DisableGPIO(boost::shared_ptr<shared::communication::IAsyncPort> serialPort)
{
   auto port = boost::dynamic_pointer_cast<shared::communication::CFT2xxSerialPort>(serialPort);

   if (port)
      port->desactivateGPIO();
}