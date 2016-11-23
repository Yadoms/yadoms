#pragma once
#include "IMessageHandler.h"
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
                   shared::event::CEventHandler& mainEventHandler,
                   int mainEvtPortDataReceived);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CMessageHandler();

   // IMessageHandler implementation
   void onReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> receivedMessage) override;
   bool send(const message::CEsp3SendPacket& sendMessage,
             boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
             boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct) override;
   void send(const message::CEsp3SendPacket& sendMessage) override;

   // [END] IMessageHandler implementation

protected:
   void setHook(boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct);

   bool waitAnswer(const boost::posix_time::time_duration& enOceanAnswerTimeout);

private:
   boost::shared_ptr<shared::communication::IAsyncPort> m_port;
   shared::event::CEventHandler& m_mainEventHandler;
   int m_mainEvtPortDataReceived;

   // Hook
   mutable boost::recursive_mutex m_hookMutex;
   boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> m_isExpectedMessageHookFct;
   boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> m_onReceiveHookFct;
   shared::event::CEventHandler m_waitAnswerEventHandler;
};

