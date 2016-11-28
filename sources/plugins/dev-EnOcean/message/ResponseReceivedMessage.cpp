#include "stdafx.h"
#include "ResponseReceivedMessage.h"

namespace message
{
   CResponseReceivedMessage::CResponseReceivedMessage(boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
      : m_returnCode(static_cast<EReturnCode>(esp3Packet->data()[0])),
        m_responseData(esp3Packet->data().begin() + 1, esp3Packet->data().end())
   {
   }

   CResponseReceivedMessage::~CResponseReceivedMessage()
   {
   }

   CResponseReceivedMessage::EReturnCode CResponseReceivedMessage::returnCode() const
   {
      return m_returnCode;
   }

   const std::vector<unsigned char>& CResponseReceivedMessage::responseData() const
   {
      return m_responseData;
   }
} // namespace message
