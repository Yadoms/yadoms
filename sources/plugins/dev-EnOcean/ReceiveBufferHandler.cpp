#include "stdafx.h"
#include "ReceiveBufferHandler.h"
#include "message/Crc8.h"
#include <shared/currentTime/Provider.h>


CReceiveBufferHandler::CReceiveBufferHandler(boost::shared_ptr<IMessageHandler> messageHandler)
   : m_lastReceivedTime(shared::currentTime::Provider().now()),
     m_messageHandler(messageHandler)
{
}

CReceiveBufferHandler::~CReceiveBufferHandler()
{
}

void CReceiveBufferHandler::push(const shared::communication::CByteBuffer& buffer)
{
   // Manage timeout
   const auto now = shared::currentTime::Provider().now();
   if (m_content.size() != 0)
   {
      // Reset data if too old
      if (now - m_lastReceivedTime > message::EnOceanInterByteTimeout)
         m_content.clear();
   }
   m_lastReceivedTime = now;

   // Add received buffer to actual content
   for (size_t idx = 0; idx < buffer.size(); ++ idx)
      m_content.push_back(buffer[idx]);

   // Send message if complete (separate aggregated messages)
   while(true)
   {
      auto completeMessage = getCompleteMessage();
      if (!completeMessage)
         break;
      m_messageHandler->onReceived(completeMessage);
   }
}

void CReceiveBufferHandler::flush()
{
   m_content.clear();
}


boost::shared_ptr<const message::CEsp3ReceivedPacket> CReceiveBufferHandler::getCompleteMessage()
{
   static const boost::shared_ptr<const message::CEsp3ReceivedPacket> uncompleteMessage;

   if (m_content.empty())
      return uncompleteMessage;

   // Remove first bytes if not sync byte
   while (!m_content.empty() && m_content[message::kOffsetSyncByte] != message::SYNC_BYTE_VALUE)
      m_content.erase(m_content.begin());

   if (m_content.size() == 1)
   {
      if (m_content[message::kOffsetSyncByte] != message::SYNC_BYTE_VALUE)
      {
         std::cerr << "Data received from EnOcean adapter : Message does not start with sync byte (0x55), wait for next sync byte" << std::endl;
         m_content.clear();
      }
      return uncompleteMessage;
   }

   if (m_content.size() < 6)
      return uncompleteMessage; // Message too small

   const size_t fullMessageSize = 6 + toWord(m_content, message::kOffsetDataLength) + m_content[message::kOffsetOptionalLength] + 1;

   if (m_content.size() < fullMessageSize)
      return uncompleteMessage; // Message too small

   // All expected bytes received, now check CRCs

   if (message::computeCrc8(m_content.begin() + message::kOffsetDataLength,
                            m_content.begin() + message::kOffsetCrc8Header) != m_content[message::kOffsetCrc8Header])
   {
      std::cerr << "Data received from EnOcean adapter : Header CRC is invalid, look for another sync byte already in buffer" << std::endl;

      for (auto byte = m_content.begin() + 1; byte != m_content.end(); ++byte)
      {
         if (*byte == message::SYNC_BYTE_VALUE)
         {
            m_content.erase(m_content.begin(), byte);
            return getCompleteMessage();
         }
      }
      // No sync byte found
      m_content.clear();
      return uncompleteMessage;
   }

   const auto offsetCrc8Data = 6 + toWord(m_content, message::kOffsetDataLength) + m_content[message::kOffsetOptionalLength];
   if (message::computeCrc8(m_content.begin() + message::kOffsetData,
                            m_content.begin() + offsetCrc8Data) != m_content[offsetCrc8Data])
   {
      std::cerr << "Data received from EnOcean adapter : Data CRC is invalid, look for another sync byte already in buffer" << std::endl;

      for (auto byte = m_content.begin() + 1; byte != m_content.end(); ++byte)
      {
         if (*byte == message::SYNC_BYTE_VALUE)
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

   auto message = boost::make_shared<message::CEsp3ReceivedPacket>(m_content);

   // Delete extracted data
   m_content.erase(m_content.begin(), m_content.begin() + fullMessageSize);

   return message;
}

