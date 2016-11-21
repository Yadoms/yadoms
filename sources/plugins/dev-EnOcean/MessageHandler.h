#pragma once
#include "IMessageHandler.h"
#include "IEnOceanReceiveThread.h"
#include <shared/communication/IAsyncPort.h>

//--------------------------------------------------------------
/// \brief	Implementation of message handler
//--------------------------------------------------------------
class CMessageHandler : public IMessageHandler
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                   boost::shared_ptr<IEnOceanReceiveThread> receiverThread);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CMessageHandler();

   // IMessageHandler implementation
   bool send(const message::CEsp3SendPacket& sendMessage,
             boost::function<bool(const message::CEsp3ReceivedPacket&)> isExpectedMessageFct,
             boost::function<void(const message::CEsp3ReceivedPacket&)> onReceiveFct) override;
   // [END] IMessageHandler implementation

private:
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;
   boost::shared_ptr<IEnOceanReceiveThread> m_receiverThread;
};

