#include "stdafx.h"
#include "TeleInfoReceiveBufferHandler.h"


CTeleInfoReceiveBufferHandler::CTeleInfoReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveDataEventId)
   :m_receiveDataEventHandler(receiveDataEventHandler), m_receiveDataEventId(receiveDataEventId)
{
}

CTeleInfoReceiveBufferHandler::~CTeleInfoReceiveBufferHandler()
{
}

void CTeleInfoReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   for (size_t idx = 0 ; idx < buffer.size() ; ++ idx)
      m_content.push_back(buffer[idx]);

   if (isComplete())
      notifyEventHandler(popNextMessage());
}

void CTeleInfoReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CTeleInfoReceiveBufferHandler::isComplete() const
{
   if (m_content.empty())
      return false;

   // The message size is provided in the first byte of the message.
   // This value counts all bytes except itself.
   // So a message is considered complete if its size is at least the value indicated
   // in the first byte + 1.
   if (m_content.size() < ((size_t)m_content[0] + 1))
      return false;

   // A message is complete
   return true;
}

boost::shared_ptr<const shared::communication::CByteBuffer> CTeleInfoReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CTeleInfoReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   // The message size is provided in the first byte of the message.
   // This value counts all bytes except itself.
   // So the message size is this value + 1.
   const size_t extractedMessageSize = m_content[0] + 1;
   boost::shared_ptr<unsigned char[]> extractedMessage(new unsigned char[extractedMessageSize]);
   for (size_t idx = 0 ; idx < extractedMessageSize ; ++ idx)
      extractedMessage[idx] = m_content[idx];

   boost::shared_ptr<const shared::communication::CByteBuffer> nextMessage(new shared::communication::CByteBuffer(extractedMessage.get(), extractedMessageSize));

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize);

   return nextMessage;
}

void CTeleInfoReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> buffer)
{
   m_receiveDataEventHandler.postEvent<const shared::communication::CByteBuffer>(m_receiveDataEventId, *buffer);
}