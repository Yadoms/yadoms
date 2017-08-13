#include "stdafx.h"
#include "LinkyFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "LinkyReceiveBufferHandler.h"
#include "Decoder.h"
#include <shared/Log.h>

CLinkyFactory::~CLinkyFactory()
{
}

boost::shared_ptr<shared::communication::IAsyncPort> CLinkyFactory::constructPort(const ILinkyConfiguration& configuration,
                                                                                  shared::event::CEventHandler& eventHandler,
                                                                                  boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler,
                                                                                  int evtPortConnectionId)
{
   YADOMS_LOG(information) << "Connecting Linky on serial port " << configuration.getSerialPort() << "..." ;

   auto port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
                                                                           boost::asio::serial_port_base::baud_rate(9600),
                                                                           boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even),
                                                                           boost::asio::serial_port_base::character_size(7),
                                                                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

   port->subscribeForConnectionEvents(eventHandler,
                                      evtPortConnectionId);

   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}

boost::shared_ptr<shared::communication::IReceiveBufferHandler> CLinkyFactory::GetBufferHandler(shared::event::CEventHandler& eventHandler,
                                                                                                int evtPortDataReceived,
                                                                                                const bool isDeveloperMode)
{
   boost::shared_ptr<shared::communication::IBufferLogger> logger;

   logger = boost::make_shared<shared::communication::CAsciiBufferLogger>("trace");

   return boost::make_shared<CLinkyReceiveBufferHandler>(eventHandler,
                                                         evtPortDataReceived,
                                                         boost::posix_time::seconds(30),
                                                         logger,
                                                         isDeveloperMode);
}

boost::shared_ptr<IDecoder> CLinkyFactory::constructDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
{
   return boost::make_shared<CDecoder>(api);
}