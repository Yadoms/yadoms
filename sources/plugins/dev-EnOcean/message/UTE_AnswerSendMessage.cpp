#include "stdafx.h"
#include "UTE_AnswerSendMessage.h"
#include <profiles/bitsetHelpers.hpp>

namespace message
{
   CUTE_AnswerSendMessage::CUTE_AnswerSendMessage(const std::string& destinationId,
                                                  bool bidirectionnalCommunication,
                                                  EResponse response,
                                                  unsigned char channelNumber,
                                                  unsigned int manufacturerId,
                                                  unsigned char type,
                                                  unsigned char func,
                                                  unsigned char rorg)
      : CEsp3SendPacket(RESPONSE),//TODO pas sûr...
        m_destinationId(destinationId),
        m_bidirectionalCommunication(bidirectionnalCommunication),
        m_response(response),
        m_command(kTeachInResponse),
        m_channelNumber(channelNumber),
        m_manufacturerId(manufacturerId),
        m_type(type),
        m_func(func),
        m_rorg(rorg)
   {
   }

   CUTE_AnswerSendMessage::~CUTE_AnswerSendMessage()
   {
   }

   boost::shared_ptr<const std::vector<unsigned char>> CUTE_AnswerSendMessage::buffer()
   {
      boost::dynamic_bitset<> db6(8);
      bitset_insert(db6, 0, 1, m_bidirectionalCommunication ? 1 : 0);
      bitset_insert(db6, 2, 2, m_response);
      bitset_insert(db6, 4, 4, m_command);

      std::vector<unsigned char> data(7);
      data[0] = bitset_to_byte(db6);
      data[1] = m_channelNumber;
      data[2] = m_manufacturerId & 0xFF;
      data[3] = (m_manufacturerId >> 8) & 0x07;
      data[4] = m_type;
      data[5] = m_func;
      data[6] = m_rorg;

      std::vector<unsigned char> optionalData(7);

      enum
         {
            kSubTelNumSend = 3
         };
      enum
         {
            kDBmSubCase = 0xFF
         };
      enum
         {
            kTelegramUnencrypted = 0
         };

      optionalData[0] = kSubTelNumSend;
      optionalData[0] = static_cast<unsigned char>(std::stoul(m_destinationId.substr(0, 2), nullptr, 16));
      optionalData[1] = static_cast<unsigned char>(std::stoul(m_destinationId.substr(2, 2), nullptr, 16));
      optionalData[2] = static_cast<unsigned char>(std::stoul(m_destinationId.substr(4, 2), nullptr, 16));
      optionalData[3] = static_cast<unsigned char>(std::stoul(m_destinationId.substr(6, 2), nullptr, 16));
      optionalData[4] = kDBmSubCase;
      optionalData[5] = kTelegramUnencrypted;

      appendData(data);
      appendOptional(optionalData);

      return CEsp3SendPacket::buffer();
   }
} // namespace message


