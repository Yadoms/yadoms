#include "stdafx.h"
#include "UTE_GigaConceptReversedReceivedMessage.h"


namespace message
{
   bool CUTE_GigaConceptReversedReceivedMessage::isCGigaConceptReversedUteMessage(const message::CRadioErp1ReceivedMessage& message)
   {
      // Try to recognize if userData is in revert order, only for device known as bugged.
      auto userData = message.userData();
      if (userData.size() == 7
         && userData[0] == 0xD2 // Profile - Rorg
         && userData[1] == 0x01 // Profile - Func
         && userData[2] == 0x08 // Profile - Type
         && userData[3] == 0x00 // Manufacturer ID MSB
         && userData[4] == 0x3E // Manufacturer ID LSB ==> GigaConceptFrance
         && userData[5] == 0xFF)
         return true;

      // Normal message
      return false;
   }

   CUTE_GigaConceptReversedReceivedMessage::CUTE_GigaConceptReversedReceivedMessage(const message::CRadioErp1ReceivedMessage& message)
      : CUTE_ReceivedMessage(message)
   {
   }

   CUTE_GigaConceptReversedReceivedMessage::~CUTE_GigaConceptReversedReceivedMessage()
   {
   }

   bool CUTE_GigaConceptReversedReceivedMessage::bidirectionalCommunication() const
   {
      return (m_erp1.userData()[6] & 0x80) ? true : false;
   }

   bool CUTE_GigaConceptReversedReceivedMessage::teachInResponseExpected() const
   {
      return (m_erp1.userData()[6] & 0x40) ? false : true;
   }

   CUTE_GigaConceptReversedReceivedMessage::ETeachInRequest CUTE_GigaConceptReversedReceivedMessage::teachInRequest() const
   {
      return static_cast<ETeachInRequest>((m_erp1.userData()[6] & 0x30) >> 4);
   }

   unsigned char CUTE_GigaConceptReversedReceivedMessage::command() const
   {
      return m_erp1.userData()[6] & 0x0F;
   }

   unsigned char CUTE_GigaConceptReversedReceivedMessage::channelNumber() const
   {
      return m_erp1.userData()[5];
   }

   unsigned int CUTE_GigaConceptReversedReceivedMessage::manufacturerId() const
   {
      return m_erp1.userData()[4] | ((m_erp1.userData()[3] & 0x07) << 8);
   }

   unsigned char CUTE_GigaConceptReversedReceivedMessage::type() const
   {
      return m_erp1.userData()[2];
   }

   unsigned char CUTE_GigaConceptReversedReceivedMessage::func() const
   {
      return m_erp1.userData()[1];
   }

   unsigned char CUTE_GigaConceptReversedReceivedMessage::rorg() const
   {
      return m_erp1.userData()[0];
   }
} // namespace message
