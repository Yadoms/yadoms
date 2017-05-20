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
      explicit CUTE_ReceivedMessage(const CRadioErp1ReceivedMessage& message);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CUTE_ReceivedMessage();

      virtual bool bidirectionalCommunication() const;
      virtual bool teachInResponseExpected() const;
      virtual ETeachInRequest teachInRequest() const;
      virtual unsigned char command() const;
      virtual unsigned char channelNumber() const;
      virtual unsigned int manufacturerId() const;
      virtual unsigned char type() const;
      virtual unsigned char func() const;
      virtual unsigned char rorg() const;
      virtual std::string senderId() const;

      protected:
      const CRadioErp1ReceivedMessage& m_erp1;
   };
} // namespace message


