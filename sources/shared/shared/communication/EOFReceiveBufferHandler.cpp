#include "stdafx.h"
#include "EOFReceiveBufferHandler.h"


namespace shared
{
   namespace communication
   {
      CEOFReceiveBufferHandler::CEOFReceiveBufferHandler(event::CEventHandler& receiveDataEventHandler,
                                                         int receiveDataEventId,
                                                         char eofCharacter)
         : m_receiveDataEventHandler(receiveDataEventHandler),
           m_receiveDataEventId(receiveDataEventId),
           m_eofCharacter(eofCharacter)
      {
      }

      CEOFReceiveBufferHandler::~CEOFReceiveBufferHandler()
      {
      }

      void CEOFReceiveBufferHandler::push(const CByteBuffer& buffer)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_contentMutex);
         for (size_t idx = 0; idx < buffer.size(); ++ idx)
         {
            auto receivedByte = buffer[idx];
            m_content.push_back(receivedByte);
            if (receivedByte == m_eofCharacter)
               notifyEventHandler(popNextMessage());
         }
      }

      void CEOFReceiveBufferHandler::flush()
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_contentMutex);
         m_content.clear();
      }

      boost::shared_ptr<const CByteBuffer> CEOFReceiveBufferHandler::popNextMessage()
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_contentMutex);
         auto extractedMessage(boost::make_shared<CByteBuffer>(m_content.size()));
         for (size_t idx = 0; idx < m_content.size(); ++ idx)
            (*extractedMessage)[idx] = m_content[idx];

         m_content.clear();

         return extractedMessage;
      }

      void CEOFReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const CByteBuffer> buffer) const
      {
         m_receiveDataEventHandler.postEvent<const CByteBuffer>(m_receiveDataEventId, *buffer);
      }
   }
} // namespace shared::communication
