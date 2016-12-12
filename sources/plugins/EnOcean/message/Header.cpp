#include "stdafx.h"
#include "Header.h"

namespace message
{
   CHeader::CHeader(const std::vector<unsigned char>& receivedBuffer)
      : m_dataLength(toWord(receivedBuffer, kOffsetDataLength)),
      m_optionalLength(receivedBuffer[kOffsetOptionalLength]),
      m_packetType(static_cast<EPacketType>(receivedBuffer[kOffsetPacketType])),
      m_offsetData(kOffsetData),
      m_offsetOptional(m_offsetData + m_dataLength)
   {
   }

   unsigned short CHeader::dataLength() const
   {
      return m_dataLength;
   }

   unsigned char CHeader::optionalLength() const
   {
      return m_optionalLength;
   }

   EPacketType CHeader::packetType() const
   {
      return m_packetType;
   }

   int CHeader::offsetData() const
   {
      return m_offsetData;
   }

   int CHeader::offsetOptional() const
   {
      return m_offsetOptional;
   }
} // namespace message


