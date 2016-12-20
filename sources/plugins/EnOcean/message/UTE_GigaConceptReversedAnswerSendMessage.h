#pragma once
#include "RadioErp1SendMessage.h"
#include "UTE_AnswerSendMessage.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	GigaConcept specific UTE message
   ///
   /// Code very-specific for a bugged device : the remote control socket DO21-12B-E from Giga-Concept.
   /// This device send and receive the userData (and only this part of the message) in reversed byte order.
   //--------------------------------------------------------------
   class CUTE_GigaConceptReversedAnswerSendMessage : public CUTE_AnswerSendMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      //--------------------------------------------------------------
      CUTE_GigaConceptReversedAnswerSendMessage(const std::string& senderId,
                                                const std::string& destinationId,
                                                unsigned char status,
                                                bool bidirectionnalCommunication,
                                                EResponse response,
                                                unsigned char channelNumber,
                                                unsigned int manufacturerId,
                                                unsigned char type,
                                                unsigned char func,
                                                unsigned char rorg);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CUTE_GigaConceptReversedAnswerSendMessage();

      boost::shared_ptr<const std::vector<unsigned char>> buffer() override;
   };
} // namespace message
