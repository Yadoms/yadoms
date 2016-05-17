#include "stdafx.h"
#include "TeleInfoReceiveBufferHandler.h"

#define TELEINFO_BUFFER 512

CTeleInfoReceiveBufferHandler::CTeleInfoReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler, int receiveDataEventId)
   :m_receiveDataEventHandler(receiveDataEventHandler), m_receiveDataEventId(receiveDataEventId), m_receptionSuspended ( false )
{
}

CTeleInfoReceiveBufferHandler::~CTeleInfoReceiveBufferHandler()
{
}

void CTeleInfoReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   if (!m_receptionSuspended)
   {
	   for (size_t idx = 0 ; idx < buffer.size() ; ++ idx)
		  m_content.push_back(buffer[idx]);

	   if (isComplete())
		  notifyEventHandler(popNextMessage());
   }
   else
	   flush(); // If the reception is suspended we flush the content
}

void CTeleInfoReceiveBufferHandler::suspend ( void )
{
	m_receptionSuspended = true;
}

void CTeleInfoReceiveBufferHandler::resume  ( void )
{
	m_receptionSuspended = false;
}

void CTeleInfoReceiveBufferHandler::flush()
{
   m_content.clear();
}

bool CTeleInfoReceiveBufferHandler::isComplete() const
{
   if ( m_content.size() < TELEINFO_BUFFER )
      return false;

   // A message is complete
   return true;
}

boost::shared_ptr<const shared::communication::CByteBuffer> CTeleInfoReceiveBufferHandler::popNextMessage()
{
   if (!isComplete())
      throw shared::exception::CException("CTeleInfoReceiveBufferHandler : Can not pop not completed message. Call isComplete to check if a message is available");

   const size_t extractedMessageSize = TELEINFO_BUFFER;
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
