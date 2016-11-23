#pragma once
#include "RadioErp1SendMessage.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean UTE answer message (Universal Teach-in)
   ///
   /// This class manages a UTE EnOcean answer message.
   //--------------------------------------------------------------
   class CUTE_AnswerSendMessage : public CRadioErp1SendMessage
   {
   public:
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
      CUTE_AnswerSendMessage();

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CUTE_AnswerSendMessage();

      void bidirectionalCommunication(bool bidirectionnalCommunication);
      void teachInResponse(EResponse response);
      void channelNumber(unsigned char channelNumber);
      void manufacturerId(unsigned int manufacturerId);
      void type(unsigned char type);
      void func(unsigned char func);
      void rorg(unsigned char rorg);

   protected:
      virtual void updateBuffer(std::vector<unsigned char>& buffer) const;

   private:
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


