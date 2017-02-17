#pragma once
#include "../profiles/generated/eep.h"


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
      enum EReturnCode
      {
         RET_OK = 0x00,
         RET_ERROR = 0x01,
         RET_NOT_SUPPORTED = 0x02,
         RET_WRONG_PARAM = 0x03,
         RET_OPERATION_DENIED = 0x04,
         RET_LOCK_SET = 0x05,
         RET_BUFFER_TO_SMALL = 0x06,
         RET_NO_FREE_BUFFER = 0x07,

         RET_SEPCIAL_RC_OFFSET = 0x80,
      };

      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] esp3Packet              The esp3 message
      //--------------------------------------------------------------
      explicit CResponseReceivedMessage(boost::shared_ptr<const CEsp3ReceivedPacket> esp3Packet);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CResponseReceivedMessage();

      EReturnCode returnCode() const;
      const std::vector<unsigned char>& responseData() const;

   private:
      EReturnCode m_returnCode;
      const std::vector<unsigned char> m_responseData;
   };
} // namespace message
