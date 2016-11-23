#pragma once
#include "RadioErp1ReceivedMessage.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean UTE message (Universal Teach-in)
   ///
   /// This class manages a UTE EnOcean message.
   //--------------------------------------------------------------
   class CUTE_ReceivedMessage
   {
   public:
      enum ETeachInRequest
      {
         kTeachInRequest = 0,
         kTeachInDeletionRequest,
         kNotSpecified,
         kNotUsed
      };

      enum ECommand
      {
         kTeachInQuery = 0
      };

      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] message                 The ERP1 radio message
      //--------------------------------------------------------------
      explicit CUTE_ReceivedMessage(const message::CRadioErp1ReceivedMessage& message);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CUTE_ReceivedMessage();

      const CRadioErp1ReceivedMessage& erp1() const;

      bool bidirectionalCommunication() const;
      bool teachInResponseExpected() const;
      ETeachInRequest teachInRequest() const;
      unsigned char command() const;
      unsigned char channelNumber() const;
      unsigned int manufacturerId() const;
      unsigned char type() const;
      unsigned char func() const;
      unsigned char rorg() const;

   private:
      const CRadioErp1ReceivedMessage& m_erp1;
   };
} // namespace message


