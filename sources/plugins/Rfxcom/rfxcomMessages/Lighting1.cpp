#include "stdafx.h"
#include "Lighting1.h"

namespace rfxcomMessages
{

CLighting1::CLighting1(unsigned char subType, unsigned char houseCode, unsigned char unitCode, unsigned char cmnd)
{
   m_buffer.LIGHTING1.packetlength = sizeof(m_buffer.LIGHTING1) - sizeof(m_buffer.LIGHTING1.packetlength);
   m_buffer.LIGHTING1.packettype = pTypeLighting1;
   m_buffer.LIGHTING1.subtype = subType;
   m_buffer.LIGHTING1.seqnbr = 0;//TODO : fournir un sequenceNumberProvider. Spec dit "Sequence number, this is not used by the RFXCOM. This can be used in the application to synchronize the commands sent with response messages. If not used in the application leave it zero."
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