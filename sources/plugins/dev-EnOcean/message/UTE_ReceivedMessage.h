#pragma once
#include "Header.h"
#include "profiles/generated-eep.h"


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

      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] esp3Packet              The esp3 message
      //--------------------------------------------------------------
      explicit CUTE_ReceivedMessage(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CUTE_ReceivedMessage();

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
      const bool m_bidirectionalCommunication;
      const bool m_teachInResponseExpected;
      const ETeachInRequest m_teachInRequest;
      const unsigned char m_command;
      const unsigned char m_channelNumber;
      const unsigned int m_manufacturerId;
      const unsigned char m_type;
      const unsigned char m_func;
      const unsigned char m_rorg;
   };
} // namespace message


