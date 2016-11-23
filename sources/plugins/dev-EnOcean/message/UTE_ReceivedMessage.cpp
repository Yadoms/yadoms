#include "stdafx.h"
#include "UTE_ReceivedMessage.h"

//TODO aligner tous les messsages en réception suivant cette syntaxe
namespace message
{
   CUTE_ReceivedMessage::CUTE_ReceivedMessage(const message::CRadioErp1ReceivedMessage& message)
      : m_erp1(message)
   {
   }

   CUTE_ReceivedMessage::~CUTE_ReceivedMessage()
   {
   }

   const CRadioErp1ReceivedMessage& CUTE_ReceivedMessage::erp1() const
   {
      return m_erp1;
   }

   bool CUTE_ReceivedMessage::bidirectionalCommunication() const
   {
      return (m_erp1.data()[0] & 0x80) ? true : false;
   }

   bool CUTE_ReceivedMessage::teachInResponseExpected() const
   {
      return (m_erp1.data()[0] & 0x40) ? false : true;
   }

   CUTE_ReceivedMessage::ETeachInRequest CUTE_ReceivedMessage::teachInRequest() const
   {
      return static_cast<ETeachInRequest>((m_erp1.data()[0] & 0x30) >> 4);
   }

   unsigned char CUTE_ReceivedMessage::command() const
   {
      return m_erp1.data()[0] & 0x0F;
   }

   unsigned char CUTE_ReceivedMessage::channelNumber() const
   {
      return m_erp1.data()[1];
   }

   unsigned int CUTE_ReceivedMessage::manufacturerId() const
   {
      return m_erp1.data()[2] | ((m_erp1.data()[3] & 0x07) << 8);
   }

   unsigned char CUTE_ReceivedMessage::type() const
   {
      return m_erp1.data()[4];
   }

   unsigned char CUTE_ReceivedMessage::func() const
   {
      return m_erp1.data()[5];
   }

   unsigned char CUTE_ReceivedMessage::rorg() const
   {
      return m_erp1.data()[6];
   }
} // namespace message


