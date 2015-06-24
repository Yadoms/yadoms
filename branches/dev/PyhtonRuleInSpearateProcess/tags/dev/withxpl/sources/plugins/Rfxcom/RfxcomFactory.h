#pragma once

#include "IRfxcomConfiguration.h"
#include "IPort.h"
#include "ITransceiver.h"

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CRfxcomFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcomFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the rfxcom
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] iOService               The plugin IO service, used by the port
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   //--------------------------------------------------------------
   static boost::shared_ptr<IPort> constructPort(
      const IRfxcomConfiguration& configuration,
      boost::asio::io_service& iOService,
      boost::shared_ptr<shared::event::CEventHandler> eventHandler,
      int evtPortConnectionId,
      int evtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	                           Create a transceiver instance
   /// \param[in] port                    Port used to exchange messages
   //--------------------------------------------------------------
   static boost::shared_ptr<ITransceiver> constructTransceiver(
      boost::shared_ptr<IPort> port);
};

