#include "stdafx.h"
#include "ZiBlueMessageHandler.h"
#include <shared/Log.h>

CZiBlueMessageHandler::CZiBlueMessageHandler(boost::shared_ptr<shared::communication::IAsyncPort> port,
                                 shared::event::CEventHandler& mainEventHandler,
                                 int mainEvtPortDataReceived)
   : m_port(port),
     m_mainEventHandler(mainEventHandler),
     m_mainEvtPortDataReceived(mainEvtPortDataReceived)
{
}

CZiBlueMessageHandler::~CZiBlueMessageHandler()
{
}


void CZiBlueMessageHandler::send(const std::string & sendMessage)
{
   if (!m_port)
      throw shared::exception::CException("Send message failed : dongle is not ready");

   m_port->sendText(sendMessage);
}

bool CZiBlueMessageHandler::send(const std::string& sendMessage, boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct, boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct)
{
   if (!m_port)
      throw shared::exception::CException("Send message failed : dongle is not ready");

   setHook(isExpectedMessageFct, onReceiveFct);
   send(sendMessage);
   return waitAnswer(boost::posix_time::seconds(5));
}

void CZiBlueMessageHandler::setHook(boost::function<bool(boost::shared_ptr<const frames::CFrame>)> isExpectedMessageFct,  boost::function<void(boost::shared_ptr<const frames::CFrame>)> onReceiveFct)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_hookMutex);
   m_isExpectedMessageHookFct = isExpectedMessageFct;
   m_onReceiveHookFct = onReceiveFct;
   m_waitAnswerEventHandler.clear();
}

bool CZiBlueMessageHandler::waitAnswer(const boost::posix_time::time_duration& enOceanAnswerTimeout)
{
   return m_waitAnswerEventHandler.waitForEvents(enOceanAnswerTimeout) != shared::event::kTimeout;
}

void CZiBlueMessageHandler::onReceived(boost::shared_ptr<const frames::CFrame> receivedMessage)
{
   YADOMS_LOG(information) << receivedMessage->toString();

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

