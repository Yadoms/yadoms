#include "stdafx.h"
#include "UTE_GigaConceptReversedAnswerSendMessage.h"
#include "../profiles/bitsetHelpers.hpp"
#include "../profiles/generated/eep.h"

namespace message
{
   CUTE_GigaConceptReversedAnswerSendMessage::CUTE_GigaConceptReversedAnswerSendMessage(const std::string& senderId,
                                                                                        const std::string& destinationId,
                                                                                        unsigned char status,
                                                                                        bool bidirectionnalCommunication,
                                                                                        EResponse response,
                                                                                        unsigned char channelNumber,
                                                                                        unsigned int manufacturerId,
                                                                                        unsigned char type,
                                                                                        unsigned char func,
                                                                                        unsigned char rorg)
      : CUTE_AnswerSendMessage(senderId,
                               destinationId,
                               status,
                               bidirectionnalCommunication,
                               response,
                               channelNumber,
                               manufacturerId,
                               type,
                               func,
                               rorg)
   {
   }

   CUTE_GigaConceptReversedAnswerSendMessage::~CUTE_GigaConceptReversedAnswerSendMessage()
   {
   }

   boost::shared_ptr<const std::vector<unsigned char>> CUTE_GigaConceptReversedAnswerSendMessage::buffer()
   {
      boost::dynamic_bitset<> db6(8);
      bitset_insert(db6, 0, 1, m_bidirectionalCommunication ? 1 : 0);
      bitset_insert(db6, 2, 2, m_response);
      bitset_insert(db6, 4, 4, m_command);

      std::vector<unsigned char> userData(7);
      userData[6] = bitset_to_byte(db6);
      userData[5] = m_channelNumber;
      userData[4] = m_manufacturerId & 0xFF;
      userData[3] = (m_manufacturerId >> 8) & 0x07;
      userData[2] = m_type;
      userData[1] = m_func;
      userData[0] = m_rorg;

      CRadioErp1SendMessage::userData(userData);

      return CRadioErp1SendMessage::buffer();
   }
} // namespace message
