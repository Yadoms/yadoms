#include "stdafx.h"
#include "Factory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/EOFReceiveBufferHandler.h>
#include <shared/communication/BufferLogger.h>
#include <shared/communication/NoBufferLogger.h>
#include "ReceiveBufferHandler.h"
#include "MessageHandler.h"


CFactory::~CFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CFactory::constructPort(const IConfiguration& configuration)
{
   YADOMS_LOG(information) << "Connecting EnOcean dongle on serial port " << configuration.getSerialPort() << "...";
   auto port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
                                                                           boost::asio::serial_port_base::baud_rate(57600),
                                                                           boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                                                                           boost::asio::serial_port_base::character_size(8),
                                                                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
   return port;
}

boost::shared_ptr<shared::communication::IReceiveBufferHandler> CFactory::constructReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler)
{
   return boost::make_shared<CReceiveBufferHandler>(messageHandler);
}

boost::shared_ptr<shared::communication::IBufferLogger> CFactory::constructBufferLogger(bool fullLog)
{
   return fullLog ?
             static_cast<boost::shared_ptr<shared::communication::IBufferLogger>>(boost::make_shared<shared::communication::CBufferLogger>()) :
             static_cast<boost::shared_ptr<shared::communication::IBufferLogger>>(boost::make_shared<shared::communication::CNoBufferLogger>());
}

boost::shared_ptr<IMessageHandler> CFactory::constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                                                     shared::event::CEventHandler& eventHandler,
                                                                     int evtPortDataReceived,
                                                                     boost::shared_ptr<shared::communication::IBufferLogger> bufferLogger)
{
   return boost::make_shared<CMessageHandler>(port,
                                              eventHandler,
                                              evtPortDataReceived,
                                              bufferLogger);
}
