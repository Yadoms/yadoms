#include "stdafx.h"
#include "ZiBlueReceiveBufferHandler.h"
#include <shared/communication/StringBuffer.h>

CZiBlueReceiveBufferHandler::CZiBlueReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveBinaryDataEventId, int receiveCommandAnswerEventId)
   : m_receiveDataEventHandler(receiveDataEventHandler), m_receiveBinaryDataEventId(receiveBinaryDataEventId), m_receiveCommandAnswerEventId(receiveCommandAnswerEventId)
{
}

CZiBlueReceiveBufferHandler::~CZiBlueReceiveBufferHandler()
{
}

void CZiBlueReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   for (auto idx = 0; idx < buffer.size(); ++idx)
      m_content.push_back(buffer[idx]);

   if (isComplete())
      notifyEventHandler(popNextMessage());
}

void CZiBlueReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CZiBlueReceiveBufferHandler::isComplete() const
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

CZiBlueReceiveBufferHandler::BufferContainer CZiBlueReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CZiBlueReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   // The message size is provided in the first byte of the message
   // (see RFXCom specifications). This value counts all bytes except itself.
   // So the message size is this value + 1.
   const size_t extractedMessageSize = m_content[0] + 1;

   BufferContainer container;
   container.bufferType = kBinaryData;
   
   boost::shared_ptr<shared::communication::CByteBuffer> extractedMessage(new shared::communication::CByteBuffer(extractedMessageSize));
   for (size_t idx = 0; idx < extractedMessageSize; ++idx)
      (*extractedMessage)[idx] = m_content[idx];

   container.buffer = extractedMessage;

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + extractedMessageSize);

   return container;
}

void CZiBlueReceiveBufferHandler::notifyEventHandler(BufferContainer & bufferContainer) const
{
   switch (bufferContainer.bufferType)
   {
   case kBinaryData:
      m_receiveDataEventHandler.postEvent<shared::communication::CByteBuffer>(m_receiveBinaryDataEventId, *bufferContainer.buffer);
      break;
   case kCommandAnswer:
      m_receiveDataEventHandler.postEvent<shared::communication::CStringBuffer>(m_receiveCommandAnswerEventId, shared::communication::CStringBuffer(*bufferContainer.buffer));
      break;
   }
   
      
}

