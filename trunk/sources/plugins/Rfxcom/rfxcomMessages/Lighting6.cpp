#include "stdafx.h"
#include "Lighting6.h"

namespace rfxcomMessages
{

CLighting6::CLighting6(unsigned short id, unsigned char groupCode, unsigned char unitCode, unsigned char cmnd, boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider)
{
   m_buffer.LIGHTING6.packetlength = sizeof(m_buffer.LIGHTING6) - sizeof(m_buffer.LIGHTING6.packetlength);
   m_buffer.LIGHTING6.packettype = pTypeLighting6;
   m_buffer.LIGHTING6.subtype = sTypeBlyss;
   m_buffer.LIGHTING6.seqnbr = seqNumberProvider->getNext();
   m_buffer.LIGHTING6.id1 = (unsigned char)((id & 0xFF00) >> 8);
   m_buffer.LIGHTING6.id2 = (unsigned char)(id & 0xFF);
   m_buffer.LIGHTING6.groupcode = groupCode;
   m_buffer.LIGHTING6.unitcode = unitCode;
   m_buffer.LIGHTING6.cmnd = cmnd;
   m_buffer.LIGHTING6.cmndseqnbr = 0;//TODO voir spec
   m_buffer.LIGHTING6.seqnbr2 = 0;//TODO voir spec
   m_buffer.LIGHTING6.rssi = 0;
   m_buffer.LIGHTING6.filler = 0;
}

CLighting6::~CLighting6()
{
}

const boost::asio::const_buffer CLighting6::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.LIGHTING6));
}
} // namespace rfxcomMessages