#pragma once
#include "UTE_ReceivedMessage.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	GigaConcept specific UTE message
   ///
   /// Code very-specific for a bugged device : the remote control socket DO21-12B-E from Giga-Concept.
   /// This device send the userData (and only this part of the message) in reversed byte order.
   /// If we recognize this message, we have to reorder bytes
   //--------------------------------------------------------------
   class CUTE_GigaConceptReversedReceivedMessage : public CUTE_ReceivedMessage
   {
   public:
      static bool isCGigaConceptReversedUteMessage(const message::CRadioErp1ReceivedMessage& message);

      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] message                 The ERP1 radio message
      //--------------------------------------------------------------
      explicit CUTE_GigaConceptReversedReceivedMessage(const message::CRadioErp1ReceivedMessage& message);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CUTE_GigaConceptReversedReceivedMessage();

      bool bidirectionalCommunication() const override;
      bool teachInResponseExpected() const override;
      ETeachInRequest teachInRequest() const override;
      unsigned char command() const override;
      unsigned char channelNumber() const override;
      unsigned int manufacturerId() const override;
      unsigned char type() const override;
      unsigned char func() const override;
      unsigned char rorg() const override;
   };
} // namespace message


