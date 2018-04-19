#include "stdafx.h"
#include "LinkyFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "LinkyDecoder.h"
#include "HistoricDecoder.h"
#include <shared/Log.h>

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

   if (type == Standard)
      baudRate = m_baudRateStandard;
   else
      baudRate = m_baudRateHistoric;

   auto port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
                                                                           boost::asio::serial_port_base::baud_rate(baudRate),
                                                                           boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even),
                                                                           boost::asio::serial_port_base::character_size(7),
                                                                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

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