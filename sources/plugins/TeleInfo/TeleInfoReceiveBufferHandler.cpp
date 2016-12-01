#include "stdafx.h"
#include "TeleInfoReceiveBufferHandler.h"

// TODO_V2 create a receiver Handler that begin a message to a STX, and send the buffer at a EOF.

CTeleInfoReceiveBufferHandler::CTeleInfoReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                             int receiveDataEventId,
                                                             size_t messageSize)
   : m_receiveDataEventHandler(receiveDataEventHandler),
     m_receiveDataEventId(receiveDataEventId),
     m_messageSize(messageSize),
     m_receptionSuspended(false)
{
}

CTeleInfoReceiveBufferHandler::~CTeleInfoReceiveBufferHandler()
{
}

void CTeleInfoReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   if (!m_receptionSuspended)
   {
      for (size_t idx = 0; idx < buffer.size(); ++ idx)
         m_content.push_back(buffer[idx]);

      if (isComplete())
         notifyEventHandler(popNextMessage());
   }
}

void CTeleInfoReceiveBufferHandler::suspend()
{
   m_receptionSuspended = true;
}

void CTeleInfoReceiveBufferHandler::resume()
{
   flush(); // flush the buffer before resume
   m_receptionSuspended = false;
}

void CTeleInfoReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CTeleInfoReceiveBufferHandler::isComplete() const
{
   if (m_content.size() < m_messageSize)
      return false;

   // A message is complete
   return true;
}

boost::shared_ptr<const shared::communication::CByteBuffer> CTeleInfoReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CTeleInfoReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   boost::shared_ptr<shared::communication::CByteBuffer> extractedMessage(new shared::communication::CByteBuffer(m_messageSize));
   for (size_t idx = 0; idx < m_messageSize; ++idx)
      (*extractedMessage)[idx] = m_content[idx];

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + m_messageSize);

   return extractedMessage;
}

void CTeleInfoReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> buffer) const
{
   m_receiveDataEventHandler.postEvent<const shared::communication::CByteBuffer>(m_receiveDataEventId,
                                                                                 *buffer);
}