#include "stdafx.h"
#include "ReceivedMessage.h"
#include "types.h"

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


   CReceivedMessage::CReceivedMessage(const std::vector<unsigned char>& receivedBuffer)
      : m_header(receivedBuffer),
        m_data(receivedBuffer.begin() + m_header.offsetData(), receivedBuffer.begin() + m_header.offsetData() + m_header.dataLength()),
        m_optional(receivedBuffer.begin() + m_header.offsetOptional(), receivedBuffer.begin() + m_header.offsetOptional() + m_header.optionalLength())
   {
   }

   CReceivedMessage::~CReceivedMessage()
   {
   }

   const CHeader& CReceivedMessage::header() const
   {
      return m_header;
   }

   const std::vector<unsigned char>& CReceivedMessage::data() const
   {
      return m_data;
   }

   const std::vector<unsigned char>& CReceivedMessage::optional() const
   {
      return m_optional;
   }
} // namespace message


