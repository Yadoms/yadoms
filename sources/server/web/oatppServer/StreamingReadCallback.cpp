#include "stdafx.h"
#include "StreamingReadCallback.h"

#include "shared/Log.h"

using namespace web::oatppServer;

unsigned int CSseClientConnection::m_connectionsCount = 0;

CSseClientConnection::CSseClientConnection(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                                           int streamingEventId)
   : m_eventHandler(std::move(eventHandler)),
     m_streamingEventId(streamingEventId)
{
   ++m_connectionsCount;
   YADOMS_LOG(information) << "SSE connections count = " << m_connectionsCount;
}

CSseClientConnection::~CSseClientConnection()
{
   --m_connectionsCount;
   YADOMS_LOG(information) << "SSE connections count = " << m_connectionsCount;
}

oatpp::String CSseClientConnection::handle() const
{
   if (m_eventHandler->waitForEvents() == m_streamingEventId)
      return "event: hello !\n"
         "data: <some data>\n\n";
   return {};
}

CStreamingReadCallback::CStreamingReadCallback(boost::shared_ptr<shared::event::CEventHandler> eventHandler,
                                               int streamingEventId)
   : m_clientConnection(boost::make_shared<CSseClientConnection>(eventHandler,
                                                                 streamingEventId))
{
}

oatpp::v_io_size CStreamingReadCallback::read(void* buffer, v_buff_size count,
                                              oatpp::async::Action& action)
{
   if (m_inlineData.bytesLeft == 0)
   {
      m_currentMessage = m_clientConnection->handle();
      if (m_currentMessage)
         m_inlineData.set(m_currentMessage->data(), m_currentMessage->size());
   }

   v_buff_size desiredToRead = m_inlineData.bytesLeft;
   if (desiredToRead > 0)
   {
      if (desiredToRead > count) desiredToRead = count;
      std::memcpy(buffer, m_inlineData.currBufferPtr, desiredToRead);
      m_inlineData.inc(desiredToRead);
      return desiredToRead;
   }

   return 0;
}
