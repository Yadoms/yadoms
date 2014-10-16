#include "stdafx.h"
#include "MegatecUpsFactory.h"
#include <shared/Log.h>
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/EOFReceiveBufferHandler.h>


CMegatecUpsFactory::~CMegatecUpsFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CMegatecUpsFactory::constructPort(
   const IMegatecUpsConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived)
{
   YADOMS_LOG(info) << "Connecting UPS on serial port " << configuration.getSerialPort() << "...";
   boost::shared_ptr<shared::communication::IAsyncPort> port(new shared::communication::CAsyncSerialPort(
      configuration.getSerialPort(),
      boost::asio::serial_port_base::baud_rate(2400)));

   port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

   boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler(
      new shared::communication::CEOFReceiveBufferHandler(eventHandler, evtPortDataReceived, MEGATEC_EOF));
   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}