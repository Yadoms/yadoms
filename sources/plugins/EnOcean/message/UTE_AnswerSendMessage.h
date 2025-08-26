#pragma once
#include "radioErp1/SendMessage.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean UTE answer message (Universal Teach-in)
   ///
   /// This class manages a UTE EnOcean answer message.
   //--------------------------------------------------------------
   class CUTE_AnswerSendMessage : public radioErp1::CSendMessage
   {
   public:
      enum
      {
         kTeachInResponse = 0x01
      };

      enum EResponse
      {
         kRequestNotAccepted = 0,
         kRequestAccepted,
         kDeletionRequestAccepted,
         kEepNotSupported
      };

      //--------------------------------------------------------------
      /// \brief	                           Constructor
      //--------------------------------------------------------------
      CUTE_AnswerSendMessage(const std::string& senderId,
                             const std::string& destinationId,
                             unsigned char status,
                             bool bidirectionnalCommunication,
                             EResponse response,
                             unsigned char channelNumber,
                             unsigned int manufacturerId,
                             unsigned char type,
                             unsigned char func,
                             unsigned char rorg);

      ~CUTE_AnswerSendMessage() override = default;

      boost::shared_ptr<const std::vector<unsigned char>> buffer() override;

   protected:
      bool m_bidirectionalCommunication;
      EResponse m_response;
      const unsigned char m_command;
      unsigned char m_channelNumber;
      unsigned int m_manufacturerId;
      unsigned char m_type;
      unsigned char m_func;
      unsigned char m_rorg;
   };
} // namespace message
