#include "stdafx.h"
#include "EnOceanMessage.h"

namespace EnOceanMessage
{
   CHeader::CHeader(const std::vector<unsigned char>& receivedBuffer)
      : m_dataLength(toWord(receivedBuffer, offsetDataLength)),
        m_optionalLength(receivedBuffer[offsetDataLength]),
        m_packetType(static_cast<EPacketType>(receivedBuffer[offsetPacketType])),
        m_offsetData(offsetData),
        m_offsetOptional(m_offsetData + offsetDataLength)
   {
   }

   CMessage::CMessage(const std::vector<unsigned char>& receivedBuffer)
      : m_header(receivedBuffer),
        m_data(receivedBuffer.begin() + m_header.m_offsetData, receivedBuffer.begin() + m_header.m_offsetData + m_header.m_dataLength),
        m_optional(receivedBuffer.begin() + m_header.m_offsetOptional, receivedBuffer.begin() + m_header.m_offsetOptional + m_header.m_optionalLength)
   {
   }

   CMessage::~CMessage()
   {
   }

   const CHeader& CMessage::header() const
   {
      return m_header;
   }

   const std::vector<unsigned char>& CMessage::data() const
   {
      return m_data;
   }

   const std::vector<unsigned char>& CMessage::optional() const
   {
      return m_optional;
   }
} // namespace EnOceanMessage


