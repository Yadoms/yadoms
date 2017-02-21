#include "stdafx.h"
#include "RfxComReceiveBufferHandler.h"


CRfxcomReceiveBufferHandler::CRfxcomReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                         int receiveDataEventId)
   : m_lastReceivedTime(shared::currentTime::Provider().now()),
   m_receiveDataEventHandler(receiveDataEventHandler),
   m_receiveDataEventId(receiveDataEventId)
{
}

CRfxcomReceiveBufferHandler::~CRfxcomReceiveBufferHandler()
{
}

void CRfxcomReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   static const auto InterByteTimeout = boost::posix_time::milliseconds(100);

   // Manage timeout
   const auto now = shared::currentTime::Provider().now();
   if (m_content.size() != 0)
   {
      // Reset data if too old
      if (now - m_lastReceivedTime > InterByteTimeout)
         m_content.clear();
   }
   m_lastReceivedTime = now;

   for (auto idx = 0; idx < buffer.size(); ++idx)
      m_content.push_back(buffer[idx]);

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
   if (m_content.size() < (static_cast<size_t>(m_content[0]) + 1))
      return false;

   // A message is complete
   return true;
}

boost::shared_ptr<const shared::communication::CByteBuffer> CRfxcomReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CRfxcomReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   // The message size is provided in the first byte of the message
   // (see RFXCom specifications). This value counts all bytes except itself.
   // So the message size is this value + 1.
   const size_t extractedMessageSize = m_content[0] + 1;
   boost::shared_ptr<shared::communication::CByteBuffer> extractedMessage(new shared::communication::CByteBuffer(extractedMessageSize));
   for (size_t idx = 0; idx < extractedMessageSize; ++idx)
      (*extractedMessage)[idx] = m_content[idx];

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize);

   return extractedMessage;
}

void CRfxcomReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const shared::communication::CByteBuffer> buffer) const
{
   m_receiveDataEventHandler.postEvent<const shared::communication::CByteBuffer>(m_receiveDataEventId, *buffer);
}

