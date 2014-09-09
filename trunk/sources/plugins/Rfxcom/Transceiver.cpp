#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "RfxcomFactory.h"
#include "rfxcomMessages/Ack.h"
#include "rfxcomMessages/Curtain1.h"
#include "rfxcomMessages/Lighting1.h"
#include "rfxcomMessages/Lighting2.h"
#include "rfxcomMessages/Lighting3.h"
#include "rfxcomMessages/Lighting6.h"
#include "rfxcomMessages/Temp.h"
#include "rfxcomMessages/TransceiverStatus.h"
#include "IncrementSequenceNumber.h"
#include "PortException.hpp"
#include "ProtocolException.hpp"


CTransceiver::CTransceiver()
   :m_seqNumberProvider(new CIncrementSequenceNumber())
{
}

CTransceiver::~CTransceiver()
{
}

const CByteBuffer CTransceiver::buildResetCmd() const
{
   // Raz sequence number
   m_seqNumberProvider->reset();

   RBUF request;
   MEMCLEAR(request.ICMND);   // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->last();
   request.ICMND.cmnd = cmdRESET;

   return CByteBuffer((BYTE*)&request.ICMND, sizeof(request.ICMND));
}

const CByteBuffer CTransceiver::buildGetStatusCmd() const
{
   RBUF request;
   MEMCLEAR(request.ICMND);   // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = cmdSTATUS;

   return CByteBuffer((BYTE*)&request.ICMND, sizeof(request.ICMND));
}

const CByteBuffer CTransceiver::buildSetModeCmd(unsigned char frequency, const IRfxcomConfiguration& configuration) const
{
   RBUF request;
   MEMCLEAR(request.ICMND);   // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = cmdSETMODE;

   // Frequency
   request.ICMND.msg1 = frequency;

   // Add protocols activation
   request.ICMND.msg3 = 0;
   if (configuration.isUNDECODEDenabled() ) request.ICMND.msg3 |= 0x80;
   if (configuration.isRFU6enabled()      ) request.ICMND.msg3 |= 0x40;
   if (configuration.isSXenabled()        ) request.ICMND.msg3 |= 0x20;
   if (configuration.isRSLenabled()       ) request.ICMND.msg3 |= 0x10;
   if (configuration.isLIGHTING4enabled() ) request.ICMND.msg3 |= 0x08;
   if (configuration.isFINEOFFSETenabled()) request.ICMND.msg3 |= 0x04;
   if (configuration.isRUBICSONenabled()  ) request.ICMND.msg3 |= 0x02;
   if (configuration.isAEenabled()        ) request.ICMND.msg3 |= 0x01;
   request.ICMND.msg4 = 0;
   if (configuration.isBLINDST1enabled()  ) request.ICMND.msg4 |= 0x80;
   if (configuration.isBLINDST0enabled()  ) request.ICMND.msg4 |= 0x40;
   if (configuration.isPROGUARDenabled()  ) request.ICMND.msg4 |= 0x20;
   if (configuration.isFS20enabled()      ) request.ICMND.msg4 |= 0x10;
   if (configuration.isLACROSSEenabled()  ) request.ICMND.msg4 |= 0x08;
   if (configuration.isHIDEKIenabled()    ) request.ICMND.msg4 |= 0x04;
   if (configuration.isLWRFenabled()      ) request.ICMND.msg4 |= 0x02;
   if (configuration.isMERTIKenabled()    ) request.ICMND.msg4 |= 0x01;
   request.ICMND.msg5 = 0;
   if (configuration.isVISONICenabled()   ) request.ICMND.msg5 |= 0x80;
   if (configuration.isATIenabled()       ) request.ICMND.msg5 |= 0x40;
   if (configuration.isOREGONenabled()    ) request.ICMND.msg5 |= 0x20;
   if (configuration.isMEIANTECHenabled() ) request.ICMND.msg5 |= 0x10;
   if (configuration.isHEEUenabled()      ) request.ICMND.msg5 |= 0x08;
   if (configuration.isACenabled()        ) request.ICMND.msg5 |= 0x04;
   if (configuration.isARCenabled()       ) request.ICMND.msg5 |= 0x02;
   if (configuration.isX10enabled()       ) request.ICMND.msg5 |= 0x01;

   return CByteBuffer((BYTE*)&request.ICMND, sizeof(request.ICMND));
}

const CByteBuffer CTransceiver::buildMessageToDevice(const shared::CDataContainer& command, const shared::CDataContainer& deviceParametersTree) const
{
   try
   {
      unsigned char deviceType = deviceParametersTree.get<unsigned char>("type");

      // Create the RFXCom message
      switch(deviceType)
      {
      case pTypeLighting1:
         return rfxcomMessages::CLighting1(command, deviceParametersTree.get<shared::CDataContainer>("content")).encode(m_seqNumberProvider);
         break;
      case pTypeLighting2:
         return rfxcomMessages::CLighting2(command, deviceParametersTree.get<shared::CDataContainer>("content")).encode(m_seqNumberProvider);
         break;
      case pTypeLighting3:
         return rfxcomMessages::CLighting3(command, deviceParametersTree.get<shared::CDataContainer>("content")).encode(m_seqNumberProvider);
         break;
      case pTypeLighting6:
         return rfxcomMessages::CLighting6(command, deviceParametersTree.get<shared::CDataContainer>("content")).encode(m_seqNumberProvider);
         break;
      case pTypeCurtain:
         return rfxcomMessages::CCurtain1(command, deviceParametersTree.get<shared::CDataContainer>("content")).encode(m_seqNumberProvider);
         break;
         //TODO compl�ter
      default:
         YADOMS_LOG(error) << "Invalid command \"" << command.serialize() << "\" : " << " unknown type " << deviceType;
         BOOST_ASSERT_MSG(false, "Invalid command (unknown type)");
         throw shared::exception::CInvalidParameter(command.serialize());
         break;
      }
   }
   catch (shared::exception::CException & e)
   {
      BOOST_ASSERT_MSG(false, "Invalid command (parameter doesn't exist)");
      std::string message = (boost::format("Invalid command \"%1%\" : %2%") % command.serialize() % e.what()).str();
      throw shared::exception::CInvalidParameter(message);
   }
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CTransceiver::decodeRfxcomMessage(const CByteBuffer& data) const
{
   const RBUF * const buf = reinterpret_cast<const RBUF* const>(data.content());

   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> message;
   switch(buf->RXRESPONSE.packettype)
   {
   case pTypeInterfaceMessage    : message.reset(new rfxcomMessages::CTransceiverStatus(*buf, m_seqNumberProvider)); break;
   case pTypeRecXmitMessage      : message.reset(new rfxcomMessages::CAck(*buf, m_seqNumberProvider));               break;
   case pTypeLighting1           : message.reset(new rfxcomMessages::CLighting1(*buf, m_seqNumberProvider));         break;
   case pTypeLighting2           : message.reset(new rfxcomMessages::CLighting2(*buf, m_seqNumberProvider));         break;
   case pTypeLighting3           : message.reset(new rfxcomMessages::CLighting3(*buf, m_seqNumberProvider));         break;
   case pTypeLighting6           : message.reset(new rfxcomMessages::CLighting6(*buf, m_seqNumberProvider));         break;
   case pTypeCurtain             : message.reset(new rfxcomMessages::CCurtain1(*buf, m_seqNumberProvider));          break;
   case pTypeTEMP                : message.reset(new rfxcomMessages::CTemp(*buf, m_seqNumberProvider));              break;
      // TODO � compl�ter
   default:
      {
         YADOMS_LOG(error) << "Invalid RfxCom message received, unknown packet type " << std::setfill('0') << std::setw(sizeof(unsigned char) * 2) << std::hex << (int)buf->RXRESPONSE.packettype;
         break;
      }
   }

   return message;
}