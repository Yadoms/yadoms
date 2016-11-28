#include "stdafx.h"
#include "MessageHandler.h"
#include "ProtocolException.hpp"


CMessageHandler::CMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                 shared::event::CEventHandler& mainEventHandler,
                                 int mainEvtPortDataReceived,
                                 boost::shared_ptr<shared::communication::IBufferLogger> bufferLogger)
   : m_port(port),
     m_mainEventHandler(mainEventHandler),
     m_mainEvtPortDataReceived(mainEvtPortDataReceived),
     m_bufferLogger(bufferLogger)
{
}

CMessageHandler::~CMessageHandler()
{
}


void CMessageHandler::send(message::CEsp3SendPacket& sendMessage)
{
   if (!m_port)
      throw CProtocolException("Send message failed : dongle is not ready");

   auto buffer = shared::communication::CByteBuffer(*sendMessage.buffer());

   m_bufferLogger->logSent(buffer);
   m_port->send(buffer);
}

bool CMessageHandler::send(message::CEsp3SendPacket& sendMessage,
                           boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                           boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct)
{
   if (!m_port)
      throw CProtocolException("Send message failed : dongle is not ready");

   setHook(isExpectedMessageFct,
           onReceiveFct);

   m_port->send(shared::communication::CByteBuffer(*sendMessage.buffer()));

   return waitAnswer(message::EnOceanAnswerTimeout);
}

void CMessageHandler::setHook(boost::function<bool(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> isExpectedMessageFct,
                              boost::function<void(boost::shared_ptr<const message::CEsp3ReceivedPacket>)> onReceiveFct)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_hookMutex);
   m_isExpectedMessageHookFct = isExpectedMessageFct;
   m_onReceiveHookFct = onReceiveFct;
   m_waitAnswerEventHandler.clear();
}

bool CMessageHandler::waitAnswer(const boost::posix_time::time_duration& enOceanAnswerTimeout)
{
   return m_waitAnswerEventHandler.waitForEvents(enOceanAnswerTimeout) != shared::event::kTimeout;
}

void CMessageHandler::onReceived(boost::shared_ptr<const message::CEsp3ReceivedPacket> receivedMessage)
{
   m_bufferLogger->logReceived(shared::communication::CByteBuffer(receivedMessage->buffer()));

   boost::lock_guard<boost::recursive_mutex> lock(m_hookMutex);
   if (m_isExpectedMessageHookFct.empty() || !m_isExpectedMessageHookFct(receivedMessage))
   {
      // Redirect to plugin
      m_mainEventHandler.postEvent(m_mainEvtPortDataReceived, receivedMessage);
      return;
   }

   m_isExpectedMessageHookFct.clear();
   auto onReceiveFct = m_onReceiveHookFct;
   m_onReceiveHookFct.clear();

   onReceiveFct(receivedMessage);

   m_waitAnswerEventHandler.postEvent(shared::event::kUserFirstId);
}

