#include "stdafx.h"
#include "UTE_ReceivedMessage.h"


namespace message
{
   CUTE_ReceivedMessage::CUTE_ReceivedMessage(const CRadioErp1ReceivedMessage& message)
      : m_erp1(message)
   {
   }

   CUTE_ReceivedMessage::~CUTE_ReceivedMessage()
   {
   }

   bool CUTE_ReceivedMessage::bidirectionalCommunication() const
   {
      return (m_erp1.userData()[0] & 0x80) ? true : false;
   }

   bool CUTE_ReceivedMessage::teachInResponseExpected() const
   {
      return (m_erp1.userData()[0] & 0x40) ? false : true;
   }

   CUTE_ReceivedMessage::ETeachInRequest CUTE_ReceivedMessage::teachInRequest() const
   {
      return static_cast<ETeachInRequest>((m_erp1.userData()[0] & 0x30) >> 4);
   }

   unsigned char CUTE_ReceivedMessage::command() const
   {
      return m_erp1.userData()[0] & 0x0F;
   }

   unsigned char CUTE_ReceivedMessage::channelNumber() const
   {
      return m_erp1.userData()[1];
   }

   unsigned int CUTE_ReceivedMessage::manufacturerId() const
   {
      return m_erp1.userData()[2] | ((m_erp1.userData()[3] & 0x07) << 8);
   }

   unsigned char CUTE_ReceivedMessage::type() const
   {
      return m_erp1.userData()[4];
   }

   unsigned char CUTE_ReceivedMessage::func() const
   {
      return m_erp1.userData()[5];
   }

   unsigned char CUTE_ReceivedMessage::rorg() const
   {
      return m_erp1.userData()[6];
   }

   std::string CUTE_ReceivedMessage::senderId() const
   {
      return m_erp1.senderId();
   }
} // namespace message


