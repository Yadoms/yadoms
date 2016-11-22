#include "stdafx.h"
#include "UTE_ReceivedMessage.h"

namespace message
{
   CUTE_ReceivedMessage::CUTE_ReceivedMessage(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
      : m_bidirectionalCommunication((esp3Packet->data()[0] & 0x80) ? true: false),
   m_teachInResponseExpected((esp3Packet->data()[0] & 0x40) ? false : true),
      m_teachInRequest(static_cast<ETeachInRequest>((esp3Packet->data()[0] & 0x30) >> 4)),
      m_command(esp3Packet->data()[0] & 0x0F),
   m_channelNumber(esp3Packet->data()[1]),
      m_manufacturerId(esp3Packet->data()[2] | ((esp3Packet->data()[3] & 0x07) << 8)),
      m_type(esp3Packet->data()[4]),
      m_func(esp3Packet->data()[5]),
      m_rorg(esp3Packet->data()[6])
   {
   }

   CUTE_ReceivedMessage::~CUTE_ReceivedMessage()
   {
   }

   bool CUTE_ReceivedMessage::bidirectionalCommunication() const
   {
      return m_bidirectionalCommunication;
   }

   bool CUTE_ReceivedMessage::teachInResponseExpected() const
   {
      return m_teachInResponseExpected;
   }

   CUTE_ReceivedMessage::ETeachInRequest CUTE_ReceivedMessage::teachInRequest() const
   {
      return m_teachInRequest;
   }

   unsigned char CUTE_ReceivedMessage::command() const
   {
      return m_command;
   }

   unsigned char CUTE_ReceivedMessage::channelNumber() const
   {
      return m_channelNumber;
   }

   unsigned int CUTE_ReceivedMessage::manufacturerId() const
   {
      return m_manufacturerId;
   }

   unsigned char CUTE_ReceivedMessage::type() const
   {
      return m_type;
   }

   unsigned char CUTE_ReceivedMessage::func() const
   {
      return m_func;
   }

   unsigned char CUTE_ReceivedMessage::rorg() const
   {
      return m_rorg;
   }


} // namespace message


