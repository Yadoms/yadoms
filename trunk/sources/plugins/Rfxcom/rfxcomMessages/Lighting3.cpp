#include "stdafx.h"
#include "Lighting3.h"

namespace rfxcomMessages
{

CLighting3::CLighting3(unsigned char system, unsigned short channel, unsigned char cmnd)
{
   m_buffer.LIGHTING3.packetlength = sizeof(m_buffer.LIGHTING3) - sizeof(m_buffer.LIGHTING3.packetlength);
   m_buffer.LIGHTING3.packettype = pTypeLighting3;
   m_buffer.LIGHTING3.subtype = sTypeKoppla;
   m_buffer.LIGHTING3.seqnbr = 0;//TODO : fournir un sequenceNumberProvider. Spec dit "Sequence number, this is not used by the RFXCOM. This can be used in the application to synchronize the commands sent with response messages. If not used in the application leave it zero."
   m_buffer.LIGHTING3.system = system;
   m_buffer.LIGHTING3.channel8_1 = (unsigned char)(channel & 0xFF);
   m_buffer.LIGHTING3.channel10_9 = (unsigned char)((channel & 0xFF00) >> 8);
   m_buffer.LIGHTING3.rssi = 0;
   m_buffer.LIGHTING3.filler = 0;
}

CLighting3::~CLighting3()
{
}

const boost::asio::const_buffer CLighting3::getBuffer() const
{
   return boost::asio::const_buffer(&m_buffer, sizeof(m_buffer.LIGHTING3));
}
} // namespace rfxcomMessages