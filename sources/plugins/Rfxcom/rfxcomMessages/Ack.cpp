#include "stdafx.h"
#include "Ack.h"

namespace rfxcomMessages
{

   CAck::CAck(const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   {
      CheckReceivedMessage(rbuf, rbufSize, pTypeRecXmitMessage, sTypeTransmitterResponse, GET_RBUF_STRUCT_SIZE(RXRESPONSE), seqNumberProvider->last());

      m_ack = rbuf.RXRESPONSE.msg == 0x00;  // Ack is OK if rbuf->RXRESPONSE.msg == 0
   }

   CAck::~CAck()
   {
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CAck::encode(boost::shared_ptr<ISequenceNumberProvider> /*seqNumberProvider*/) const
   {
      throw shared::exception::CInvalidParameter("Ack is a read-only message, can not be encoded");
   }

   void CAck::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize
   }

   const std::string& CAck::getDeviceName() const
   {
      static const std::string emptyString;
      return emptyString;
   }

} // namespace rfxcomMessages