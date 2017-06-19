#include "stdafx.h"
#include "LinkyFactory.h"
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/AsciiBufferLogger.h>
#include "LinkyReceiveBufferHandler.h"
#include "LinkytrxHelpers.h"
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
                                                                           boost::asio::serial_port_base::baud_rate(LINKY_BAUD_RATE),
                                                                           boost::asio::serial_port_base::parity(LINKY_PARITY),
                                                                           boost::asio::serial_port_base::character_size(LINKY_CARACTER_SIZE),
                                                                           boost::asio::serial_port_base::stop_bits(LINKY_STOP_BITS),
                                                                           boost::asio::serial_port_base::flow_control(LINKY_FLOW_CONTROL));

   port->subscribeForConnectionEvents(eventHandler,
                                      evtPortConnectionId);

   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}

boost::shared_ptr<shared::communication::IReceiveBufferHandler> CLinkyFactory::GetBufferHandler(shared::event::CEventHandler& eventHandler,
                                                                                                int evtPortDataReceived)
{
   boost::shared_ptr<shared::communication::IBufferLogger> logger;

   logger = boost::make_shared<shared::communication::CAsciiBufferLogger>("trace");

   return boost::make_shared<CLinkyReceiveBufferHandler>(eventHandler,
                                                         evtPortDataReceived,
                                                         boost::posix_time::seconds(30),
                                                         logger);
}

boost::shared_ptr<IDecoder> CLinkyFactory::constructDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
{
   return boost::make_shared<CDecoder>(api);
}