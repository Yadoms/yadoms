#include "stdafx.h"
#include "EnOceanReceiveThread.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "message/ReceivedMessage.h"


// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

// Event IDs
enum
{
   kEvtPortConnection = shared::event::kUserFirstId,
   kEvtPortDataReceived,
};


CEnOceanReceiveThread::CEnOceanReceiveThread(shared::event::CEventHandler& mainEventHandler,
                                             int mainEvtPortConnection,
                                             int mainEvtPortDataReceived)
   : m_mainEventHandler(mainEventHandler),
     m_mainEvtPortConnection(mainEvtPortConnection),
     m_mainEvtPortDataReceived(mainEvtPortDataReceived)
{
}

CEnOceanReceiveThread::~CEnOceanReceiveThread()
{
   CEnOceanReceiveThread::stop();
}

void CEnOceanReceiveThread::start()
{
   m_thread = boost::thread(&CEnOceanReceiveThread::doWork, this);
}

void CEnOceanReceiveThread::stop()
{
   m_thread.interrupt();
   m_thread.timed_join(boost::posix_time::seconds(20));
}

shared::event::CEventHandler& CEnOceanReceiveThread::getEventHandler()
{
   return m_eventHandler;
}

int CEnOceanReceiveThread::getEvtPortConnection()
{
   return kEvtPortConnection;
}

int CEnOceanReceiveThread::getEvtPortDataReceived()
{
   return kEvtPortDataReceived;
}

void CEnOceanReceiveThread::setHook(boost::function<bool(const message::CEsp3ReceivedPacket&)> isExpectedMessageFct,
                                    boost::function<void(const message::CEsp3ReceivedPacket&)> onReceiveFct)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_hookMutex);
   m_isExpectedMessageHookFct = isExpectedMessageFct;
   m_onReceiveHookFct = onReceiveFct;
   m_waitAnswerEventHandler.clear();
}

bool CEnOceanReceiveThread::waitAnswer(const boost::posix_time::time_duration& enOceanAnswerTimeout)
{
   return m_waitAnswerEventHandler.waitForEvents(enOceanAnswerTimeout) != shared::event::kTimeout;
}

void CEnOceanReceiveThread::doWork()
{
   try
   {
      while (true)
      {
         // Wait for an event
         switch (m_eventHandler.waitForEvents())
         {
         case kEvtPortConnection:
            // Redirect to plugin
            m_mainEventHandler.postEvent(m_mainEvtPortConnection, m_eventHandler.getEventData<bool>());
            break;

         case kEvtPortDataReceived:
            {
               auto message = m_eventHandler.getEventData<const message::CEsp3ReceivedPacket>();
               boost::lock_guard<boost::recursive_mutex> lock(m_hookMutex);
               if (m_isExpectedMessageHookFct || !m_isExpectedMessageHookFct(message))
               {
                  // Redirect to plugin
                  m_mainEventHandler.postEvent(m_mainEvtPortDataReceived, message);
                  return;
               }

               m_isExpectedMessageHookFct.clear();
               m_onReceiveHookFct.clear();
               m_waitAnswerEventHandler.postEvent(shared::event::kUserFirstId);

               m_onReceiveHookFct(message);

               break;
            }

         default:
            break;
         }
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}

