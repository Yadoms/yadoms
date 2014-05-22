#include "stdafx.h"
#include "Curtain1.h"

namespace rfxcomMessages
{

CCurtain1::CCurtain1(unsigned char houseCode, unsigned char unitCode, unsigned char cmnd)
{
   m_buffer.CURTAIN1.packetlength = sizeof(m_buffer.CURTAIN1) - sizeof(m_buffer.CURTAIN1.packetlength);
   m_buffer.CURTAIN1.packettype = pTypeCurtain;
   m_buffer.CURTAIN1.subtype = sTypeHarrison;
   m_buffer.CURTAIN1.seqnbr = 0;//TODO : fournir un sequenceNumberProvider. Spec dit "Sequence number, this is not used by the RFXCOM. This can be used in the application to synchronize the commands sent with response messages. If not used in the application leave it zero."
   m_buffer.CURTAIN1.housecode = houseCode;
   m_buffer.CURTAIN1.unitcode = unitCode;
   m_buffer.CURTAIN1.cmnd = cmnd;
   m_buffer.CURTAIN1.filler = 0;//TODO attendre le .h v6.19, il y a des choses à mettre dans cet octet (voir doc)
}

CCurtain1::~CCurtain1()
{
}

const boost::asio::const_buffer CCurtain1::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.CURTAIN1));
}
} // namespace rfxcomMessages