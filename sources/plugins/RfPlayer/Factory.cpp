#include "stdafx.h"
#include "Factory.h"
#include "ReceiveBufferHandler.h"
#include "MessageHandler.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/Log.h>

CFactory::~CFactory()
{

}

boost::shared_ptr<shared::communication::IAsyncPort> CFactory::constructPort(const CConfiguration& configuration)
{
   YADOMS_LOG(information) << "Connecting RfPlayer on serial port " << configuration.getSerialPort() << "...";
   return boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
      boost::asio::serial_port_base::baud_rate(115200),
      boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
      boost::asio::serial_port_base::character_size(8),
      boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
      boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::hardware),
      boost::posix_time::seconds(30),
      true
      );
}

boost::shared_ptr<shared::communication::IReceiveBufferHandler> CFactory::constructReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler)
{
   return boost::make_shared<CReceiveBufferHandler>(messageHandler);
}

boost::shared_ptr<IMessageHandler> CFactory::constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port, shared::event::CEventHandler& eventHandler, int evtPortDataReceived)
{
   return boost::make_shared<CMessageHandler>(port, eventHandler, evtPortDataReceived);
}
