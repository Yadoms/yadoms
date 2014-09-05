#pragma once

#include "IRfxcomConfiguration.h"
#include "IAsyncPort.h"
#include "IPortLogger.h"
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
   /// \param[in] logger                  The port logger (can be null if no logger is to be used)
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<IAsyncPort> constructPort(
      const IRfxcomConfiguration& configuration,
      shared::event::CEventHandler& eventHandler,
      int evtPortConnectionId,
      int evtPortDataReceived,
      boost::shared_ptr<IPortLogger> logger);

   //--------------------------------------------------------------
   /// \brief	                           Create a transceiver instance
   /// \return                            The created transceiver
   //--------------------------------------------------------------
   static boost::shared_ptr<ITransceiver> constructTransceiver();

   //--------------------------------------------------------------
   /// \brief	                           Create a port logger instance. This object will log all what is transceived by the port.
   /// \return                            The created port Logger (can be null if no logger available)
   //--------------------------------------------------------------
   static boost::shared_ptr<IPortLogger> constructPortLogger();
};

