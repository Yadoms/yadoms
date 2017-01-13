#pragma once

#include <shared/communication/IAsyncPort.h>
#include "ZiBlueConfiguration.h"

//--------------------------------------------------------------
/// \brief	Configuration of the xpl plugin
/// \note   To implement configuration, we have to derive from shared::plugin::configuration::CConfiguration
//--------------------------------------------------------------
class CZiBlueFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CZiBlueFactory();

   //--------------------------------------------------------------
   /// \brief	                           Create a port instance and connect to the rfxcom
   /// \param[in] configuration           Plugin instance configuration (contains the serial port)
   /// \param[in] eventHandler            The event handler to receive notifications from port
   /// \param[in] evtPortConnectionId     The event id raised on connection events
   /// \param[in] evtPortDataReceived     The event id raised on data receive events
   /// \return                            The created port
   //--------------------------------------------------------------
   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const CZiBlueConfiguration& configuration, shared::event::CEventHandler& eventHandler, int evtPortConnectionId, int evtPortDataReceived);


};

