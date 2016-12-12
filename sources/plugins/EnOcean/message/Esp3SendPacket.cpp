#include "stdafx.h"
#include "Esp3SendPacket.h"
#include "Crc8.h"

namespace message
{
   CEsp3SendPacket::CEsp3SendPacket(EPacketType packetType)
      : m_packetType(packetType)
   {
   }

   CEsp3SendPacket::~CEsp3SendPacket()
   {
   }

   EPacketType CEsp3SendPacket::packetType() const
   {
      return m_packetType;
   }

   void CEsp3SendPacket::data(const std::vector<unsigned char>& data)
   {
      m_data = data;
   }

   void CEsp3SendPacket::optionalData(const std::vector<unsigned char>& optionalData)
   {
      m_optionalData = optionalData;
   }

   boost::shared_ptr<const std::vector<unsigned char>> CEsp3SendPacket::buffer()
   {
      auto buffer = boost::make_shared<std::vector<unsigned char>>(6 + m_data.size() + m_optionalData.size() + 1);

      // Header
      size_t index = 0;
      (*buffer)[index++] = SYNC_BYTE_VALUE;
      (*buffer)[index++] = (m_data.size() >> 8) & 0xFF;
      (*buffer)[index++] = m_data.size() & 0x0FF;
      (*buffer)[index++] = m_optionalData.size() & 0x0FF;
      (*buffer)[index++] = static_cast<unsigned char>(m_packetType);
      (*buffer)[index++] = computeCrc8(buffer->begin() + kOffsetDataLength,
                                       buffer->begin() + kOffsetCrc8Header);

      // Data
      for (const auto dataByte : m_data)
         (*buffer)[index++] = dataByte;

      // Optional
      for (const auto optionalDataByte : m_optionalData)
         (*buffer)[index++] = optionalDataByte;

      // CRC8D
      (*buffer)[index] = computeCrc8(buffer->begin() + kOffsetData,
                                     buffer->begin() + kOffsetData + m_data.size() + m_optionalData.size());

      return buffer;
   }
} // namespace message
