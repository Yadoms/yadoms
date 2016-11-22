#pragma once
#include "Header.h"
#include "profiles/generated-eep.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean RADIO_ERP1 message
   ///
   /// This class manages a RADIO_ERP1 EnOcean message.
   //--------------------------------------------------------------
   class CRadioErp1ReceivedMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] esp3Packet              The esp3 message
      //--------------------------------------------------------------
      explicit CRadioErp1ReceivedMessage(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CRadioErp1ReceivedMessage();

      CRorgs::ERorgIds rorg() const;
      unsigned int senderId() const;
      std::string senderIdAsString() const;
      const std::vector<unsigned char>& data() const;
      unsigned char status() const;

   protected:
      static std::string senderIdToString(unsigned int senderId);

   private:
      const CRorgs::ERorgIds m_rorg;
      const unsigned int m_senderId;
      const std::string m_senderIdAsString;
      const unsigned char m_status;
      const std::vector<unsigned char> m_data;
   };
} // namespace message


