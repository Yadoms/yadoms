#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "rfxcomMessages/Lighting1.h"
#include "rfxcomMessages/Lighting3.h"
#include "rfxcomMessages/Lighting6.h"
#include "rfxcomMessages/Curtain1.h"
#include "rfxcomMessages/Temp.h"
#include "IncrementSequenceNumber.h"
#include "PortException.hpp"
#include "ProtocolException.hpp"


CTransceiver::CTransceiver(const IRfxcomConfiguration& configuration, boost::shared_ptr<IPort> port)
   :m_configuration(configuration), m_port(port), m_seqNumberProvider(new CIncrementSequenceNumber())
{
}

CTransceiver::~CTransceiver()
{
}

void CTransceiver::processReset()
{
   // Reset the RFXCom.
   // See the RFXCom SDK specification for more information about this sequence

   m_seqNumberProvider->reset();
   m_port->send(buildRfxcomCommand(cmdRESET));
   boost::this_thread::sleep(boost::posix_time::milliseconds(50));
   m_port->flush();
   const CTransceiverStatus status = sendCommand(buildRfxcomCommand(cmdSTATUS));

   YADOMS_LOG(info) << "RFXCom status, type (" << status.rfxcomTypeToString() << "), firmware version (" << status.getFirmwareVersion() << ")";
   status.traceEnabledProtocols();

   if (status.needConfigurationUpdate(m_configuration))
   {
      // Update active protocols list
      const CTransceiverStatus newStatus = sendCommand(buildRfxcomSetModeCommand(status.getRfxcomType(), m_configuration));// Don't change the frequency (TODO voir si Domoticz le fait, si oui, il faudra le faire)
      if (newStatus.needConfigurationUpdate(m_configuration))
         throw CProtocolException("Error configuring RfxCom, configuration was not taken into account");
   }
}

CByteBuffer CTransceiver::buildRfxcomCommand(unsigned char command) const
{
   RBUF request;
   MEMCLEAR(request.ICMND);   // For better performance, just clear the needed sub-structure of RBUF

   request.ICMND.packetlength = ENCODE_PACKET_LENGTH(ICMND);
   request.ICMND.packettype = pTypeInterfaceControl;
   request.ICMND.subtype = sTypeInterfaceCommand;
   request.ICMND.seqnbr = m_seqNumberProvider->next();
   request.ICMND.cmnd = command;

   return CByteBuffer((BYTE*)&request.ICMND, sizeof(request.ICMND));
}

CByteBuffer CTransceiver::buildRfxcomSetModeCommand(unsigned char frequency, const IRfxcomConfiguration& configuration) const
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

CTransceiverStatus CTransceiver::sendCommand(const CByteBuffer& cmd)
{
   CByteBuffer answer = m_port->sendAndReceive(cmd);

   // Check answer

   // - Check message size
   if (answer.size() != IRESPONSE_size)
   {
      throw CProtocolException("Error reading status, invalid message size");
   }

   const RBUF* rbuf = (const RBUF*)answer.content();

   // - Check message data
   if (rbuf->IRESPONSE.packetlength != (IRESPONSE_size - 1) ||
      rbuf->IRESPONSE.packettype != pTypeInterfaceMessage ||
      rbuf->IRESPONSE.subtype != sTypeInterfaceResponse ||
      rbuf->IRESPONSE.seqnbr != m_seqNumberProvider->last() ||
      rbuf->IRESPONSE.cmnd != cmdSTATUS)
   {
      throw CProtocolException("Error reading status, invalid data received");
   }

   CTransceiverStatus status(*rbuf);
   return status;
}

bool CTransceiver::checkAcknowledge(const CByteBuffer& answer)
{
   // - Check message size
   if (answer.size() != RXRESPONSE_size)
   {
      YADOMS_LOG(error) << "Error reading status, invalid message size";
      return false;
   }

   const RBUF* rbuf = (const RBUF*)answer.content();

   // - Check message data
   if (rbuf->RXRESPONSE.packetlength != (RXRESPONSE_size - 1) ||
      rbuf->RXRESPONSE.packettype != pTypeRecXmitMessage ||
      rbuf->RXRESPONSE.subtype != sTypeTransmitterResponse ||
      rbuf->RXRESPONSE.seqnbr != m_seqNumberProvider->last() ||
      rbuf->RXRESPONSE.msg != 0x00)  // Ack OK
   {
      YADOMS_LOG(error) << "Error acknowledge, invalid data received";
      return false;
   }

   YADOMS_LOG(info) << "RFXCom acknowledge";

   return true;
}

void CTransceiver::send(const std::string& command, const std::string& deviceParameters)
{
   try
   {
      // Send message
      CByteBuffer answer = m_port->sendAndReceive(buildRfxcomMessage(command, deviceParameters));

      // Check acknowledge
      checkAcknowledge(answer);
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Error sending command to RFXCom : " << e.what();
      BOOST_ASSERT_MSG(false, "Error sending command to RFXCom");
   }
}

const CByteBuffer CTransceiver::buildRfxcomMessage(const std::string& command, const std::string& deviceParameters) const
{
   try
   {
      shared::CDataContainer deviceParametersTree(deviceParameters);
      unsigned char deviceType = deviceParametersTree.get<unsigned char>("type");

      // Create the RFXCom message
      switch(deviceType)
      {
      case pTypeLighting1:
         return rfxcomMessages::CLighting1(command, deviceParametersTree.get<shared::CDataContainer>("content")).encode(m_seqNumberProvider);
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
         //TODO compléter
      default:
         YADOMS_LOG(error) << "Invalid command \"" << command << "\" : " << " unknown type " << deviceType;
         BOOST_ASSERT_MSG(false, "Invalid command (unknown type)");
         throw shared::exception::CInvalidParameter(command);
         break;
      }
   }
   catch (shared::exception::CException & e)
   {
      BOOST_ASSERT_MSG(false, "Invalid command (parameter doesn't exist)");
      throw shared::exception::CInvalidParameter("Invalid command \"" + command + "\" : " + e.what());
   }
}

void CTransceiver::historize(boost::shared_ptr<yApi::IYadomsApi> context, const CByteBuffer& data) const
{
   const RBUF * const buf = reinterpret_cast<const RBUF* const>(data.content());

   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> sensorMessage;
   switch(buf->RXRESPONSE.packettype)
   {
   case pTypeLighting1:sensorMessage.reset(new rfxcomMessages::CLighting1(*buf)); break;
   case pTypeLighting3:sensorMessage.reset(new rfxcomMessages::CLighting3(*buf)); break;
   case pTypeLighting6:sensorMessage.reset(new rfxcomMessages::CLighting6(*buf)); break;
   case pTypeCurtain:sensorMessage.reset(new rfxcomMessages::CCurtain1(*buf)); break;
   case pTypeTEMP:sensorMessage.reset(new rfxcomMessages::CTemp(*buf)); break;
      // TODO à compléter
   default:
      {
         YADOMS_LOG(error) << "Invalid RfxCom message received, unknown packet type " << std::setfill('0') << std::setw(sizeof(unsigned char) * 2) << std::hex << (int)buf->RXRESPONSE.packettype;
         return;
      }
   }
   sensorMessage->historizeData(context);
}