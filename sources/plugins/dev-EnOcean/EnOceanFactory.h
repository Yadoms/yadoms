#pragma once

#include "IEnOceanConfiguration.h"
#include <shared/communication/IAsyncPort.h>


//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CEnOceanFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CEnOceanFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the dongle
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const IEnOceanConfiguration& configuration,
                                                                             shared::event::CEventHandler& eventHandler,
                                                                             int evtPortConnectionId,
                                                                             int evtPortDataReceived);
};

