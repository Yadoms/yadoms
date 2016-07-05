#include "stdafx.h"
#include "EnOceanReceiveBufferHandler.h"
#include "EnOceanCrc8.h"


CEnOceanReceiveBufferHandler::CEnOceanReceiveBufferHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                                           int receiveDataEventId)
   : m_receiveDataEventHandler(receiveDataEventHandler),
     m_receiveDataEventId(receiveDataEventId),
   m_lastReceivedTime(shared::currentTime::Provider().now())
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
   if (m_content.size() != 0)
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
   static const boost::shared_ptr<const EnOceanMessage::CMessage> uncompleteMessage;

   if (m_content.empty())
      return uncompleteMessage;

   // Remove first bytes if not sync byte
   while(!m_content.empty() && m_content[EnOceanMessage::kOffsetSyncByte] != EnOceanMessage::SYNC_BYTE_VALUE)
      m_content.erase(m_content.begin());

   if (m_content.size() == 1)
   {
      if (m_content[EnOceanMessage::kOffsetSyncByte] != EnOceanMessage::SYNC_BYTE_VALUE)
      {
         std::cerr << "Data received from EnOcean adapter : Message does not start with sync byte (0x55), wait for next sync byte" << std::endl;
         m_content.clear();
      }
      return uncompleteMessage;
   }

   if (m_content.size() < 6)
      return uncompleteMessage; // Message too small

   const auto fullMessageSize = 6 + toWord(m_content, EnOceanMessage::kOffsetDataLength) + m_content[EnOceanMessage::kOffsetOptionalLength] + 1;

   if (m_content.size() < fullMessageSize)
      return uncompleteMessage; // Message too small

   // All expected bytes received, now check CRCs

   if (computeCrc8(m_content,
                   EnOceanMessage::kOffsetDataLength,
                   EnOceanMessage::kOffsetCrc8Header) != m_content[EnOceanMessage::kOffsetCrc8Header])
   {
      std::cerr << "Data received from EnOcean adapter : Header CRC is invalid, look for another sync byte already in buffer" << std::endl;

      for (auto byte = m_content.begin() + 1; byte != m_content.end(); ++byte)
      {
         if (*byte == EnOceanMessage::SYNC_BYTE_VALUE)
         {
            m_content.erase(m_content.begin(), byte);
            return getCompleteMessage();
         }
      }
      // No sync byte found
      m_content.clear();
      return uncompleteMessage;
   }

   const auto offsetCrc8Data = 6 + toWord(m_content, EnOceanMessage::kOffsetDataLength) + m_content[EnOceanMessage::kOffsetOptionalLength];
   if (computeCrc8(m_content,
                   EnOceanMessage::kOffsetData,
                   offsetCrc8Data) != m_content[offsetCrc8Data])
   {
      std::cerr << "Data received from EnOcean adapter : Data CRC is invalid, look for another sync byte already in buffer" << std::endl;

      for (auto byte = m_content.begin() + 1; byte != m_content.end(); ++byte)
      {
         if (*byte == EnOceanMessage::SYNC_BYTE_VALUE)
         {
            m_content.erase(m_content.begin(), byte);
            return getCompleteMessage();
         }
      }
      // No sync byte found
      m_content.clear();
      return uncompleteMessage;
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

