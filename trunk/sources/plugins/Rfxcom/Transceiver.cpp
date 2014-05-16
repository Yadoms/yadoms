#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>


// Macro helpers to access RBUF parts
// - Get buffer pointer and size
#define BUFFER_AND_SIZE(structure) &structure,sizeof(structure)
// - to clear a part of RBUF
#define MEMCLEAR(structure) memset(&structure,0,sizeof(structure));
// - Log
#define LOG_WRITE(structure) YADOMS_LOG(debug)<<" RFXCom <<< "<<msgToString(BUFFER_AND_SIZE(structure));
#define LOG_READ(structure) YADOMS_LOG(debug)<<" RFXCom >>> "<<msgToString(BUFFER_AND_SIZE(structure));

CTransceiver::CTransceiver(boost::shared_ptr<IPort> port)
   :m_port(port), m_seqNumber(0)
{
   MEMCLEAR(m_request);
}

CTransceiver::~CTransceiver()
{
}

void CTransceiver::reset()
{
   sendCommand(cmdRESET);

   // TODO on doit purger tous les messages arrivés dans les 500ms

}

void CTransceiver::sendCommand(unsigned char command)
{
   MEMCLEAR(m_request.ICMND);

   m_request.ICMND.packetlength = sizeof(m_request.ICMND);
   m_request.ICMND.packettype = pTypeInterfaceControl;
   m_request.ICMND.subtype = sTypeInterfaceCommand;
   m_request.ICMND.seqnbr = m_seqNumber;
   m_request.ICMND.cmnd = command;

   LOG_WRITE(m_request.ICMND);

   m_port->send(boost::asio::const_buffer(BUFFER_AND_SIZE(m_request.ICMND)));
}

std::string CTransceiver::msgToString(const void* ptr, size_t size) const
{
   std::ostringstream ss;
   const unsigned char* ucharPtr = reinterpret_cast<const unsigned char*> (ptr);
   for (size_t idx = 0 ; idx < size ; ++ idx)
      ss << std::fixed << std::setprecision(2) << std::hex << ucharPtr[idx] << " ";

   return ss.str();
}