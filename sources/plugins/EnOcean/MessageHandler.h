#pragma once
#include "IMessageHandler.h"
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/IBufferLogger.h>

//--------------------------------------------------------------
/// \brief	Implementation of message handler
//--------------------------------------------------------------
class CMessageHandler : public IMessageHandler
{
public:
   CMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                   shared::event::CEventHandler& mainEventHandler,
                   int mainEvtPortDataReceived,
                   boost::shared_ptr<shared::communication::IBufferLogger> bufferLogger);
   virtual ~CMessageHandler();

   // IMessageHandler implementation
   void onReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> receivedMessage) override;
   bool send(message::CEsp3SendPacket& sendMessage,
             boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
             boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct) override;
   void send(message::CEsp3SendPacket& sendMessage) override;

   // [END] IMessageHandler implementation

protected:
   void setHook(boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct);

   bool waitAnswer(const boost::posix_time::time_duration& enOceanAnswerTimeout);

private:
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;
   shared::event::CEventHandler& m_mainEventHandler;
   int m_mainEvtPortDataReceived;
   boost::shared_ptr<shared::communication::IBufferLogger> m_bufferLogger;

   // Hook
   mutable boost::recursive_mutex m_hookMutex;
   boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> m_isExpectedMessageHookFct;
   boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> m_onReceiveHookFct;
   shared::event::CEventHandler m_waitAnswerEventHandler;
};

