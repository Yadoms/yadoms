#include "stdafx.h"
#include "MessageHandler.h"
#include "ProtocolException.hpp"


CMessageHandler::CMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                 boost::shared_ptr<IEnOceanReceiveThread> receiverThread)
   : m_port(port),
     m_receiverThread(receiverThread)
{
}

CMessageHandler::~CMessageHandler()
{
}


bool CMessageHandler::send(const message::CEsp3SendPacket& sendMessage,
                           boost::function<bool(const message::CEsp3ReceivedPacket&)> isExpectedMessageFct,
                           boost::function<void(const message::CEsp3ReceivedPacket&)> onReceiveFct)
{
   if (!m_port)
      throw CProtocolException("Send message failed : dongle is not ready");

   m_receiverThread->setHook(isExpectedMessageFct,
                             onReceiveFct);

   m_port->send(sendMessage.buildBuffer());

   return m_receiverThread->waitAnswer(message::EnOceanAnswerTimeout);
}

