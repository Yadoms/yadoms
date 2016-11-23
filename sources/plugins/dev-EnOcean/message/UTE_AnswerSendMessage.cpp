#include "stdafx.h"
#include "UTE_AnswerSendMessage.h"

namespace message
{
   CUTE_AnswerSendMessage::CUTE_AnswerSendMessage()
      : m_bidirectionalCommunication(false),
        m_response(kEepNotSupported),
        m_command(/*TODO*/),
        m_channelNumber(0),
        m_manufacturerId(0),
        m_type(0),
        m_func(0),
        m_rorg(0)
   {
   }

   CUTE_AnswerSendMessage::~CUTE_AnswerSendMessage()
   {
   }

   void CUTE_AnswerSendMessage::bidirectionalCommunication(bool bidirectionnalCommunication)
   {
      m_bidirectionalCommunication = bidirectionnalCommunication;
   }

   void CUTE_AnswerSendMessage::teachInResponse(EResponse response)
   {
      m_response = response;
   }

   void CUTE_AnswerSendMessage::channelNumber(unsigned char channelNumber)
   {
      m_channelNumber = channelNumber;
   }

   void CUTE_AnswerSendMessage::manufacturerId(unsigned int manufacturerId)
   {
      m_manufacturerId = manufacturerId;
   }

   void CUTE_AnswerSendMessage::type(unsigned char type)
   {
      m_type = type;
   }

   void CUTE_AnswerSendMessage::func(unsigned char func)
   {
      m_func = func;
   }

   void CUTE_AnswerSendMessage::rorg(unsigned char rorg)
   {
      m_rorg = rorg;
   }

   void CUTE_AnswerSendMessage::updateBuffer(std::vector<unsigned char>& buffer) const
   {
      //TODO positionner data, header, etc...

      CEsp3SendPacket::updateBuffer(buffer);
   }
} // namespace message


