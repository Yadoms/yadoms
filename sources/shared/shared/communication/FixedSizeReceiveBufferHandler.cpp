#include "stdafx.h"
#include "FixedSizeReceiveBufferHandler.h"


namespace shared
{
   namespace communication
   {
      CFixedSizeReceiveBufferHandler::CFixedSizeReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                                     int receiveDataEventId,
                                                                     size_t messageSize)
         : m_receiveDataEventHandler(receiveDataEventHandler),
           m_receiveDataEventId(receiveDataEventId),
           m_messageSize(messageSize)
      {
         if (m_messageSize < 1)
            throw exception::CException("Invalid message size");
      }

      CFixedSizeReceiveBufferHandler::~CFixedSizeReceiveBufferHandler()
      {
      }

      void CFixedSizeReceiveBufferHandler::push(const CByteBuffer& buffer)
      {
         for (size_t idx = 0; idx < buffer.size(); ++ idx)
            m_content.push_back(buffer[idx]);

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
         if (!isComplete())
            throw exception::CException("CFixedSizeReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

         auto extractedMessage(boost::make_shared<CByteBuffer>(m_messageSize));
         for (size_t idx = 0; idx < m_messageSize; ++ idx)
            (*extractedMessage)[idx] = m_content[idx];

         // Delete extracted data
         m_content.erase(m_content.begin(), m_content.begin() + m_messageSize);

         return extractedMessage;
      }

      void CFixedSizeReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const CByteBuffer> buffer) const
      {
         m_receiveDataEventHandler.postEvent<const CByteBuffer>(m_receiveDataEventId, *buffer);
      }
   }
} // namespace shared::communication
