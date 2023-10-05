#include "stdafx.h"
#include "StreamingReadCallback.h"

#include "shared/Log.h"

using namespace web::oatppServer;


unsigned int CStreamingReadCallback::m_connectionsCount = 0;

CStreamingReadCallback::CStreamingReadCallback(std::shared_ptr<rest::service::ISseConnectionHandler> connectionHandler)
   : m_connectionHandler(std::move(connectionHandler))
{
   ++m_connectionsCount;
   YADOMS_LOG(information) << "SSE connections count = " << m_connectionsCount;
}

CStreamingReadCallback::~CStreamingReadCallback()
{
   --m_connectionsCount;
   YADOMS_LOG(information) << "SSE connections count = " << m_connectionsCount;
}

oatpp::v_io_size CStreamingReadCallback::read(void* buffer,
                                              v_buff_size count,
                                              oatpp::async::Action& action)
{
   if (m_inlineData.bytesLeft == 0)
   {
      m_currentMessage = m_connectionHandler->handle();
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
