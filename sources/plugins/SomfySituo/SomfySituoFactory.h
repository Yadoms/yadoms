#pragma once

#include "ISomfySituoConfiguration.h"
#include <shared/communication/IAsyncPort.h>
#include "SomfySituoReceiveBufferHandler.h"

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CSomfySituoFactory
{
public: 
   virtual ~CSomfySituoFactory();

   static boost::shared_ptr<shared::communication::IAsyncPort> constructPort(
      const ISomfySituoConfiguration& configuration,
      shared::event::CEventHandler& eventHandler,
      boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler,
      int evtPortConnectionId);

   static boost::shared_ptr<CSomfySituoReceiveBufferHandler> GetBufferHandler(
      shared::event::CEventHandler& eventHandler,
      int evtPortDataReceived,
      bool isDeveloperMode);
};
