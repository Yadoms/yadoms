#include "stdafx.h"
#include <shared/Log.h>
#include "Ack.h"
#include <shared/exception/NotImplemented.hpp>

namespace rfxcomMessages
{

CAck::CAck(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   CheckReceivedMessage(rbuf, pTypeRecXmitMessage, sTypeTransmitterResponse, GET_RBUF_STRUCT_SIZE(RXRESPONSE), seqNumberProvider->last());

   m_ack = rbuf.RXRESPONSE.msg == 0x00;  // Ack is OK if rbuf->RXRESPONSE.msg == 0
}                         

CAck::~CAck()
{
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CAck::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Ack is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Ack is a read-only message, can not be encoded");
}

void CAck::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   // Nothing to historize
}

const std::string& CAck::getDeviceName() const
{
   return shared::CStringExtension::EmptyString;
}

} // namespace rfxcomMessages