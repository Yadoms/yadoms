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


   CReceivedEsp3Packet::CReceivedEsp3Packet(const std::vector<unsigned char>& receivedBuffer)
      : m_header(receivedBuffer),
        m_data(receivedBuffer.begin() + m_header.offsetData(), receivedBuffer.begin() + m_header.offsetData() + m_header.dataLength()),
        m_optional(receivedBuffer.begin() + m_header.offsetOptional(), receivedBuffer.begin() + m_header.offsetOptional() + m_header.optionalLength())
   {
   }

   CReceivedEsp3Packet::~CReceivedEsp3Packet()
   {
   }

   const CHeader& CReceivedEsp3Packet::header() const
   {
      return m_header;
   }

   const std::vector<unsigned char>& CReceivedEsp3Packet::data() const
   {
      return m_data;
   }

   const std::vector<unsigned char>& CReceivedEsp3Packet::optional() const
   {
      return m_optional;
   }


   CRadioErp1Message::CRadioErp1Message(const CReceivedEsp3Packet& esp3Packet)
      : m_rorg(esp3Packet.data()[0]),
        m_senderId(esp3Packet.data()[esp3Packet.data().size() - 5] << 24 | esp3Packet.data()[esp3Packet.data().size() - 4] << 16 | esp3Packet.data()[esp3Packet.data().size() - 3] << 8 | esp3Packet.data()[esp3Packet.data().size() - 2]),
        m_status(esp3Packet.data()[esp3Packet.data().size() - 1]),
        m_data(esp3Packet.data().begin() + 1, esp3Packet.data().begin() + esp3Packet.data().size() - 6)
   {
   }

   CRadioErp1Message::~CRadioErp1Message()
   {
   }

   unsigned char CRadioErp1Message::rorg() const
   {
      return m_rorg;
   }

   unsigned int CRadioErp1Message::senderId() const
   {
      return m_senderId;
   }

   const std::vector<unsigned char>& CRadioErp1Message::data() const
   {
      return m_data;
   }


   C4BSMessage::C4BSMessage(const CRadioErp1Message& erp1)
      : m_db3(erp1.data()[1]),
        m_db2(erp1.data()[2]),
        m_db1(erp1.data()[3]),
        m_db0(erp1.data()[4])
   {
   }

   C4BSMessage::~C4BSMessage()
   {
   }

   unsigned char C4BSMessage::db3() const
   {
      return m_db3;
   }

   unsigned char C4BSMessage::db2() const
   {
      return m_db2;
   }

   unsigned char C4BSMessage::db1() const
   {
      return m_db1;
   }

   unsigned char C4BSMessage::db0() const
   {
      return m_db0;
   }
} // namespace message


