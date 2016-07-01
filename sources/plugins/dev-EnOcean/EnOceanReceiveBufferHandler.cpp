#include "stdafx.h"
#include "EnOceanReceiveBufferHandler.h"
#include "EnOceanCrc8.h"


CEnOceanReceiveBufferHandler::CEnOceanReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                           int receiveDataEventId)
   : m_receiveDataEventHandler(receiveDataEventHandler),
     m_receiveDataEventId(receiveDataEventId)
{
}

CEnOceanReceiveBufferHandler::~CEnOceanReceiveBufferHandler()
{
}

static const auto timeout = boost::posix_time::milliseconds(100);

void CEnOceanReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   // Manage timeout
   const auto now = shared::currentTime::Provider().now();
   if (buffer.size() != 0)
   {
      // Reset data if too old
      if (now - m_lastReceivedTime > timeout)
         m_content.clear();
   }
   m_lastReceivedTime = now;

   // Add received buffer to actual content
   for (auto idx = 0; idx < buffer.size(); ++ idx)
      m_content.push_back(buffer[idx]);

   // Send message if complete
   auto completeMessage = getCompleteMessage();
   if (!!completeMessage)
      notifyEventHandler(completeMessage);
}

void CEnOceanReceiveBufferHandler::flush()
{
   m_content.clear();
}


boost::shared_ptr<const EnOceanMessage::CMessage> CEnOceanReceiveBufferHandler::getCompleteMessage()
{
   static const boost::shared_ptr<const EnOceanMessage::CMessage> invalidMessage;

   if (m_content.empty())
      return invalidMessage;

   if (m_content.size() == 1)
   {
      if (m_content[EnOceanMessage::offsetSyncByte] != EnOceanMessage::SYNC_BYTE_VALUE)
         m_content.clear(); // Message is not valid, wait for next sync byte
      return invalidMessage;
   }

   if (m_content.size() < 6)
      return invalidMessage; // Message too small

   const auto fullMessageSize = 6 + toWord(m_content, EnOceanMessage::offsetDataLength) + m_content[EnOceanMessage::offsetOptionalLength] + 1;

   if (m_content.size() < fullMessageSize)
      return invalidMessage; // Message too small

   // All expected bytes received, now check CRCs

   if (computeCrc8(m_content,
                   EnOceanMessage::offsetDataLength,
                   EnOceanMessage::offsetCrc8Header - 1) != m_content[EnOceanMessage::offsetCrc8Header])
   {
      // Crc invalid, look for another sync byte already in buffer
      for (auto byte = m_content.begin(); byte != m_content.end(); ++byte)
      {
         if (*byte == EnOceanMessage::SYNC_BYTE_VALUE)
         {
            m_content.erase(m_content.begin(), byte);
            return getCompleteMessage();
         }
      }
      // No sync byte found
      m_content.clear();
      return invalidMessage;
   }

   const auto offsetCrc8Data = 6 + toWord(m_content, EnOceanMessage::offsetDataLength) + m_content[EnOceanMessage::offsetOptionalLength];
   if (computeCrc8(m_content,
                   EnOceanMessage::offsetData,
                   offsetCrc8Data - 1) != m_content[offsetCrc8Data])
   {
      // Crc invalid, look for another sync byte already in buffer
      for (auto byte = m_content.begin(); byte != m_content.end(); ++byte)
      {
         if (*byte == EnOceanMessage::SYNC_BYTE_VALUE)
         {
            m_content.erase(m_content.begin(), byte);
            return getCompleteMessage();
         }
      }
      // No sync byte found
      m_content.clear();
      return invalidMessage;
   }

   // The message is complete

   auto message = boost::make_shared<EnOceanMessage::CMessage>(m_content);

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + fullMessageSize);

   return message;
}

void CEnOceanReceiveBufferHandler::notifyEventHandler(boost::shared_ptr<const EnOceanMessage::CMessage> message) const
{
   m_receiveDataEventHandler.postEvent<const EnOceanMessage::CMessage>(m_receiveDataEventId, *message);
}

