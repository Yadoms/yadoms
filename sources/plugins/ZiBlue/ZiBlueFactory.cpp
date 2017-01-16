#include "stdafx.h"
#include "ZiBlueFactory.h"
#include "ZiBlueReceiveBufferHandler.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/Log.h>

CZiBlueFactory::~CZiBlueFactory()
{

}

boost::shared_ptr<shared::communication::IAsyncPort> CZiBlueFactory::constructPort(const CZiBlueConfiguration& configuration, shared::event::CEventHandler& eventHandler, int evtPortConnectionId, int evtPortBinaryDataReceived, int evtPortCommandAnswerReceived)
{
   boost::shared_ptr<shared::communication::IAsyncPort> port;
   YADOMS_LOG(information) << "Connecting ZiBlue on serial port " << configuration.getSerialPort() << "...";
   port.reset(new shared::communication::CAsyncSerialPort(  configuration.getSerialPort(), 
                                                            boost::asio::serial_port_base::baud_rate(115200), 
                                                            boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                                                            boost::asio::serial_port_base::character_size(8),
                                                            boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                            boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::hardware),
                                                            boost::posix_time::seconds(30),
                                                            true
                                                         ));

   port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

   boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler(new CZiBlueReceiveBufferHandler(eventHandler, evtPortBinaryDataReceived, evtPortCommandAnswerReceived));
   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}