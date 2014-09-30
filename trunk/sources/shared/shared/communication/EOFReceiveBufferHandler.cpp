#include "stdafx.h"
#include "EOFReceiveBufferHandler.h"


namespace shared { namespace communication {

CEOFReceiveBufferHandler::CEOFReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveDataEventId, char eofCaracter)
   :m_receiveDataEventHandler(receiveDataEventHandler), m_receiveDataEventId(receiveDataEventId), m_eofCaracter(eofCaracter)
{
}

CEOFReceiveBufferHandler::~CEOFReceiveBufferHandler()
{
}

void CEOFReceiveBufferHandler::push(const CByteBuffer& buffer)
{
   for (size_t idx = 0 ; idx < buffer.size() ; ++ idx)
   {
      unsigned char receivedByte = buffer.content()[idx];
      m_content.push_back(receivedByte);
      if (receivedByte == m_eofCaracter)
         notifyEventHandler(popNextMessage());
   }
}

void CEOFReceiveBufferHandler::flush()
{
   m_content.clear();
}

boost::shared_ptr<const CByteBuffer> CEOFReceiveBufferHandler::popNextMessage()
{
   boost::shared_ptr<unsigned char[]> extractedMessage(new unsigned char[m_content.size()]);
   for (size_t idx = 0 ; idx < m_content.size() ; ++ idx)
      extractedMessage[idx] = m_content[idx];

   boost::shared_ptr<const CByteBuffer> nextMessage(new CByteBuffer(extractedMessage.get(), m_content.size()));

   m_content.clear();

   return nextMessage;
}

void CEOFReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const CByteBuffer> buffer)
{
   m_receiveDataEventHandler.postEvent<const CByteBuffer>(m_receiveDataEventId, *buffer);
}

} } // namespace shared::communication
