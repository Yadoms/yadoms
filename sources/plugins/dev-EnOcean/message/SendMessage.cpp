#include "stdafx.h"
#include "SendMessage.h"
#include "Crc8.h"

namespace message
{
   CEsp3SendPacket::CEsp3SendPacket(EPacketType packetType)
      :m_packetType(packetType)
   {
   }

   CEsp3SendPacket::~CEsp3SendPacket()
   {
   }

   EPacketType CEsp3SendPacket::packetType() const
   {
      return m_packetType;
   }

   shared::communication::CByteBuffer CEsp3SendPacket::buildBuffer() const
   {
      std::vector<unsigned char> tempBuffer;
      updateBuffer(tempBuffer);
      return shared::communication::CByteBuffer(tempBuffer);
   }

   void CEsp3SendPacket::appendData(const std::vector<unsigned char>& data)
   {
      m_data.insert(m_data.end(), data.begin(), data.end());
   }

   void CEsp3SendPacket::appendOptional(const std::vector<unsigned char>& data)
   {
      m_optional.insert(m_data.end(), data.begin(), data.end());
   }

   void CEsp3SendPacket::updateBuffer(std::vector<unsigned char>& buffer) const
   {
      buffer.clear();

      // Header
      buffer.push_back(SYNC_BYTE_VALUE);
      buffer.push_back((m_data.size() >> 8) & 0xFF);
      buffer.push_back(m_data.size() & 0x0FF);
      buffer.push_back(m_optional.size() & 0x0FF);
      buffer.push_back(static_cast<unsigned char>(m_packetType));
      buffer.push_back(computeCrc8(buffer.begin() + kOffsetDataLength,
         buffer.begin() + kOffsetCrc8Header));

      // Data
      buffer.insert(buffer.end(), m_data.begin(), m_data.end());

      // Optional
      buffer.insert(buffer.end(), m_optional.begin(), m_optional.end());

      // CRC8D
      buffer.push_back(computeCrc8(buffer.begin() + kOffsetData,
         buffer.begin() + kOffsetData + m_data.size() + m_optional.size()));
   }
} // namespace message


