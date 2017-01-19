#include "stdafx.h"
#include "ZiBlueFactory.h"
#include "ZiBlueReceiveBufferHandler.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/Log.h>
#include "ZiBlueMessageHandler.h"

CZiBlueFactory::~CZiBlueFactory()
{

}

boost::shared_ptr<shared::communication::IAsyncPort> CZiBlueFactory::constructPort(const CZiBlueConfiguration& configuration)
{
   YADOMS_LOG(information) << "Connecting ZiBlue on serial port " << configuration.getSerialPort() << "...";
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



boost::shared_ptr<shared::communication::IReceiveBufferHandler> CZiBlueFactory::constructReceiveBufferHandler(boost::shared_ptr<IZiBlueMessageHandler> messageHandler)
{
   return boost::make_shared<CZiBlueReceiveBufferHandler>(messageHandler);
}


boost::shared_ptr<IZiBlueMessageHandler> CZiBlueFactory::constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port, shared::event::CEventHandler& eventHandler, int evtPortDataReceived)
{
   return boost::make_shared<CZiBlueMessageHandler>(port, eventHandler, evtPortDataReceived);
}
