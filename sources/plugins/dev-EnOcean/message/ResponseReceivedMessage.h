#pragma once
#include "../profiles/generated-eep.h"


namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean RESPONSE message
   ///
   /// This class manages a RESPONSE EnOcean message.
   //--------------------------------------------------------------
   class CResponseReceivedMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] esp3Packet              The esp3 message
      //--------------------------------------------------------------
      explicit CResponseReceivedMessage(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CResponseReceivedMessage();

      unsigned char returnCode() const;
      const std::vector<unsigned char>& responseData() const;

   private:
      unsigned char m_returnCode;
      const std::vector<unsigned char> m_responseData;
   };
} // namespace message


