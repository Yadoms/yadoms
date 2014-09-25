#include "stdafx.h"
#include "RfxComReceiveBufferHandler.h"


CRfxcomReceiveBufferHandler::CRfxcomReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveDataEventId)
   :m_receiveDataEventHandler(receiveDataEventHandler), m_receiveDataEventId(receiveDataEventId)
{
}

CRfxcomReceiveBufferHandler::~CRfxcomReceiveBufferHandler()
{
}

void CRfxcomReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   for (size_t idx = 0 ; idx < buffer.size() ; ++ idx)
      m_content.push_back(buffer.content()[idx]);

   if (isComplete())
      notifyEventHandler(popNextMessage());
}

void CRfxcomReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CRfxcomReceiveBufferHandler::isComplete() const
{
   if (m_content.empty())
      return false;

   // The message size is provided in the first byte of the message
   // (see RFXCom specifications). This value counts all bytes except itself.
   // So a message is considered complete if its size is at least the value indicated
   // in the first byte + 1.
   if (m_content.size() < ((size_t)m_content[0] + 1))
      return false;

   // A message is complete
   return true;
}

boost::shared_ptr<const shared::communication::CByteBuffer> CRfxcomReceiveBufferHandler::popNextMessage()
{
   BOOST_ASSERT_MSG(isComplete(), "CRfxcomReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   // The message size is provided in the first byte of the message
   // (see RFXCom specifications). This value counts all bytes except itself.
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

void CRfxcomReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> buffer)
{
   m_receiveDataEventHandler.postEvent<const shared::communication::CByteBuffer>(m_receiveDataEventId, *buffer);
}