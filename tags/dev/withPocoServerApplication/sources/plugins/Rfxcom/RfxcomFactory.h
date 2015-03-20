#pragma once

#include "IRfxcomConfiguration.h"
#include <shared/communication/IAsyncPort.h>
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
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(
      const IRfxcomConfiguration& configuration,
      shared::event::CEventHandler& eventHandler,
      int evtPortConnectionId,
      int evtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	                           Create a transceiver instance
   /// \return                            The created transceiver
   //--------------------------------------------------------------
   static boost::shared_ptr<ITransceiver> constructTransceiver();
};

