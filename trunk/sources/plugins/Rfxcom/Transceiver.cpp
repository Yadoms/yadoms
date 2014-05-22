#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/xpl/XplException.h>
#include "xplMessages/X10Basic.h"
#include "rfxcomMessages/IRfxcomMessage.h"


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

void CTransceiver::sendReset()
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

void CTransceiver::send(const shared::xpl::CXplMessage& xplMessage)
{
   BOOST_ASSERT_MSG(xplMessage.getTypeIdentifier() == shared::xpl::CXplMessage::kXplCommand, "Message should be xpl-cmnd");

   try
   {
      boost::shared_ptr<rfxcomMessages::IRfxcomMessage> rfxcomMsg = createRfxcomMessage(xplMessage);

      m_port->send(rfxcomMsg->getBuffer());
   }
   catch (shared::xpl::CXplException& e)
   {
      YADOMS_LOG(error) << "Error decoding XPL message : " << e.what() << ", message : " << xplMessage.toString();
      BOOST_ASSERT_MSG(false, "Error decoding XPL message");
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Error decoding XPL message : " << e.what() << ", message : " << xplMessage.toString();
      BOOST_ASSERT_MSG(false, "Error decoding XPL message");
   }
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CTransceiver::createRfxcomMessage(const shared::xpl::CXplMessage& xplMessage) const
{
   const std::string& classId = xplMessage.getMessageSchemaIdentifier().getClassId();
   const std::string& typeId = xplMessage.getMessageSchemaIdentifier().getTypeId();
   
   boost::shared_ptr<xplMessages::IXplMessage> xplMsg;

   if (classId == "x10" && typeId == "basic")
      xplMsg.reset(new xplMessages::CXplMsgX10Basic(xplMessage));
   //TODO compléter
   else
      throw shared::exception::CInvalidParameter("Unsupported XPL message for RFXCom : " + xplMessage.toString());

   // Convert message from XPL to RFXCom
   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> msg(xplMsg->toRfxComMessage()); //TODO erreurs à gérer ?

   return msg;
}