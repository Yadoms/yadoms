#include "stdafx.h"
#include "MessageHandler.h"
#include <shared/Log.h>

CMessageHandler::CMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port, shared::event::CEventHandler& mainEventHandler, int mainEvtPortDataReceived)
   : m_port(port), m_mainEventHandler(mainEventHandler), m_mainEvtPortDataReceived(mainEvtPortDataReceived)
{
}

CMessageHandler::~CMessageHandler()
{
}


void CMessageHandler::send(const std::string & sendMessage)
{
   if (!m_port)
      throw shared::exception::CException("Send message failed : dongle is not ready");

   YADOMS_LOG(information) << " RfPlayer Command >>> " << sendMessage;
   m_port->sendText(sendMessage);
}

bool CMessageHandler::send(const std::string& sendMessage, boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct)
{
   if (!m_port)
      throw shared::exception::CException("Send message failed : dongle is not ready");

   setHook(isExpectedMessageFct, onReceiveFct);
   send(sendMessage);
   return waitAnswer(boost::posix_time::seconds(5));
}

void CMessageHandler::setHook(boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct,  boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_hookMutex);
   m_isExpectedMessageHookFct = isExpectedMessageFct;
   m_onReceiveHookFct = onReceiveFct;
   m_waitAnswerEventHandler.clear();
}

bool CMessageHandler::waitAnswer(const boost::posix_time::time_duration& answerTimeout)
{
   return m_waitAnswerEventHandler.waitForEvents(answerTimeout) != shared::event::kTimeout;
}

void CMessageHandler::onReceived(boost::shared_ptr<const frames::CFrame> receivedMessage)
{
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

