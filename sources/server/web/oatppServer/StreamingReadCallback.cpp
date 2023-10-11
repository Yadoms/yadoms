#include "stdafx.h"
#include "StreamingReadCallback.h"

#include "shared/Log.h"
#include "shared/currentTime/Provider.h"

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
   static const auto HandleTimeout(boost::posix_time::seconds(5));

   if (m_inlineData.bytesLeft == 0)
   {
      const auto evt = m_connectionHandler->handle(HandleTimeout);

      if (evt.first.empty())
      {
         m_currentMessage =
            "event: ping\n"
            "data: {\"time\": \"" + boost::posix_time::to_iso_extended_string(shared::currentTime::Provider().now()) + "\"}\n\n";
      }
      else
      {
         m_currentMessage =
            "event: " + evt.first + "\n"
            "data: " + evt.second + "\n\n";
      }

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
