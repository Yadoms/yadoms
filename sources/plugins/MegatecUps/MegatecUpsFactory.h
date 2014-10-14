#pragma once

#include "IMegatecUpsConfiguration.h"
#include <shared/communication/IAsyncPort.h>


//--------------------------------------------------------------
/// \brief	Megatec protocol End Of Frame character
//--------------------------------------------------------------
#define MEGATEC_EOF "\r"

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CMegatecUpsFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CMegatecUpsFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the UPS
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(
      const IMegatecUpsConfiguration& configuration,
      shared::event::CEventHandler& eventHandler,
      int evtPortConnectionId,
      int evtPortDataReceived);
};

