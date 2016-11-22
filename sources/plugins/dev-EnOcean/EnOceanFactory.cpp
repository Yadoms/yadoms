#include "stdafx.h"
#include "EnOceanFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/EOFReceiveBufferHandler.h>
#include "EnOceanReceiveBufferHandler.h"
#include "MessageHandler.h"


CEnOceanFactory::~CEnOceanFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CEnOceanFactory::constructPort(const IEnOceanConfiguration& configuration,
                                                                                    shared::event::CEventHandler& eventHandler,
                                                                                    int evtPortConnectionId,
                                                                                    int evtPortDataReceived)
{
   YADOMS_LOG(information) << "Connecting EnOcean dongle on serial port " << configuration.getSerialPort() << "...";
   auto port = boost::make_shared<shared::communication::CAsyncSerialPort>(configuration.getSerialPort(),
                                                                           boost::asio::serial_port_base::baud_rate(57600),
                                                                           boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                                                                           boost::asio::serial_port_base::character_size(8),
                                                                           boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                                                           boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

   port->subscribeForConnectionEvents(eventHandler,
                                      evtPortConnectionId);

   auto messageHandler = constructMessageHandler(port,
                                                 eventHandler,
                                                 evtPortDataReceived);

   port->setReceiveBufferHandler(constructReceiveBufferHandler(messageHandler));

   return port;
}

boost::shared_ptr<shared::communication::IReceiveBufferHandler> CEnOceanFactory::constructReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler)
{
   return boost::make_shared<CEnOceanReceiveBufferHandler>(messageHandler);
}

boost::shared_ptr<IMessageHandler> CEnOceanFactory::constructMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                                                            shared::event::CEventHandler& eventHandler,
                                                                            int evtPortDataReceived)
{
   return boost::make_shared<CMessageHandler>(port,
                                              eventHandler,
                                              evtPortDataReceived);
}

