#include "stdafx.h"
#include "FixedSizeReceiveBufferHandler.h"


namespace shared { namespace communication {

CFixedSizeReceiveBufferHandler::CFixedSizeReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveDataEventId, size_t messageSize)
   :m_receiveDataEventHandler(receiveDataEventHandler), m_receiveDataEventId(receiveDataEventId), m_messageSize(messageSize)
{
   BOOST_ASSERT_MSG(m_messageSize >= 1, "Invalid message size");
}

CFixedSizeReceiveBufferHandler::~CFixedSizeReceiveBufferHandler()
{
}

void CFixedSizeReceiveBufferHandler::push(const CByteBuffer& buffer)
{
   for (size_t idx = 0 ; idx < buffer.size() ; ++ idx)
      m_content.push_back(buffer.content()[idx]);

   if (isComplete())
      notifyEventHandler(popNextMessage());
}

void CFixedSizeReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CFixedSizeReceiveBufferHandler::isComplete() const
{
   if (m_content.empty())
      return false;

   if (m_content.size() < m_messageSize)
      return false;

   // A message is complete
   return true;
}

boost::shared_ptr<const CByteBuffer> CFixedSizeReceiveBufferHandler::popNextMessage()
{
   BOOST_ASSERT_MSG(isComplete(), "CFixedSizeReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   boost::shared_ptr<unsigned char[]> extractedMessage(new unsigned char[m_messageSize]);
   for (size_t idx = 0 ; idx < m_messageSize ; ++ idx)
      extractedMessage[idx] = m_content[idx];

   boost::shared_ptr<const CByteBuffer> nextMessage(new CByteBuffer(extractedMessage.get(), m_messageSize));

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + m_messageSize);

   return nextMessage;
}

void CFixedSizeReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const CByteBuffer> buffer)
{
   m_receiveDataEventHandler.postEvent<const CByteBuffer>(m_receiveDataEventId, *buffer);
}

} } // namespace shared::communication
