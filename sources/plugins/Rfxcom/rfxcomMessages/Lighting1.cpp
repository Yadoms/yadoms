#include "stdafx.h"
#include "Lighting1.h"

namespace rfxcomMessages
{

CLighting1::CLighting1(unsigned char subType, unsigned char houseCode, unsigned char unitCode, unsigned char cmnd, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
{
   m_buffer.LIGHTING1.packetlength = sizeof(m_buffer.LIGHTING1) - sizeof(m_buffer.LIGHTING1.packetlength);
   m_buffer.LIGHTING1.packettype = pTypeLighting1;
   m_buffer.LIGHTING1.subtype = subType;
   m_buffer.LIGHTING1.seqnbr = seqNumberProvider->getNext();//TODO : faire un vrai sequence number provider qui incrémente ?
   m_buffer.LIGHTING1.housecode = houseCode;
   m_buffer.LIGHTING1.unitcode = unitCode;
   m_buffer.LIGHTING1.cmnd = cmnd;
   m_buffer.LIGHTING1.rssi = 0;
   m_buffer.LIGHTING1.filler = 0;
}

CLighting1::~CLighting1()
{
}

const boost::asio::const_buffer CLighting1::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.LIGHTING1));
}
} // namespace rfxcomMessages