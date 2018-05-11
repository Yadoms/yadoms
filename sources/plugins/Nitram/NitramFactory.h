#pragma once

#include "INitramConfiguration.h"
#include <shared/communication/IAsyncPort.h>

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CNitramFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CNitramFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the UPS
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const INitramConfiguration& configuration,
                                                                             shared::event::CEventHandler& eventHandler,
                                                                             int evtPortConnectionId,
                                                                             int evtPortDataReceived);
};