#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "rfxcomMessages/Lighting1.h"
#include "rfxcomMessages/Lighting3.h"
#include "rfxcomMessages/Lighting6.h"
#include "rfxcomMessages/Curtain1.h"
#include "IncrementSequenceNumber.h"
#include "PortException.hpp"


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

   try
   {
      m_seqNumberProvider->reset();
      m_port->send(buildRfxcomCommand(cmdRESET));
      boost::this_thread::sleep(boost::posix_time::milliseconds(50));
      m_port->flush();
      requestStatus();

      //TODO
      //if (activeProtocols != m_configuration.getActiveProtocols())
      //{
      //   // Update active protocols list
      //   m_port->send(buildRfxcomCommand(cmdSETMODE, m_configuration.getActiveProtocols()));
      //}
   }
   catch (CPortException& e)
   {
      YADOMS_LOG(error) << "Error resetting RFXCom transceiver : " << e.what();
   }
}

CByteBuffer CTransceiver::buildRfxcomCommand(unsigned char command)
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

bool CTransceiver::requestStatus()
{
   CByteBuffer answer = m_port->sendAndReceive(buildRfxcomCommand(cmdSTATUS));

   // Check answer

   // - Check message size
   if (answer.size() != IRESPONSE_size)
   {
      YADOMS_LOG(error) << "Error reading status, invalid message size";
      return false;
   }

   const RBUF* rbuf = (const RBUF*)answer.content();

   // - Check message data
   if (rbuf->IRESPONSE.packetlength != (IRESPONSE_size - 1) ||
      rbuf->IRESPONSE.packettype != pTypeInterfaceMessage ||
      rbuf->IRESPONSE.subtype != sTypeInterfaceResponse ||
      rbuf->IRESPONSE.seqnbr != m_seqNumberProvider->last() ||
      rbuf->IRESPONSE.cmnd != cmdSTATUS)
   {
      YADOMS_LOG(error) << "Error reading status, invalid data received";
      return false;
   }

   YADOMS_LOG(info) << "RFXCom status, type (" << rfxcomTypeToString(rbuf->IRESPONSE.msg1) << "), firmware version (" << (int)rbuf->IRESPONSE.msg2 << ")";

   TraceRfxComConfiguredProtocols(*rbuf);

   return true;
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

const std::string CTransceiver::rfxcomTypeToString(const unsigned char rfxcomType)
{
   static const std::map<unsigned char, std::string> RfxcomTypes = boost::assign::map_list_of
      (recType310       , "310MHz"                 )
      (recType315       , "315MHz"                 )
      (recType43392     , "433.92MHz receiver only")
      (trxType43392     , "433.92MHz transceiver"  )
      (0x54             , "433.42MHz"              )     // No constant is defined in rfxtrx.h v6.19
      (recType86800     , "868.00MHz"              )
      (recType86800FSK  , "868.00MHz FSK"          )
      (recType86830     , "868.30MHz"              )
      (recType86830FSK  , "868.30MHz FSK"          )
      (recType86835     , "868.35MHz"              )
      (recType86835FSK  , "868.35MHz FSK"          )
      (recType86895     , "868.95MHz"              );

   std::map<unsigned char, std::string>::const_iterator itRfxcomTypes = RfxcomTypes.find(rfxcomType);
   if (itRfxcomTypes == RfxcomTypes.end())
      return boost::lexical_cast<std::string>(rfxcomType);

   return itRfxcomTypes->second;
}

void CTransceiver::TraceRfxComConfiguredProtocols(const RBUF& rbuf)
{
   YADOMS_LOG(info) << "RFXCom configured protocols :";

   if (rbuf.IRESPONSE.AEenabled        ) YADOMS_LOG(info) << "   - AE Blyss";
   if (rbuf.IRESPONSE.RUBICSONenabled  ) YADOMS_LOG(info) << "   - Rubicson";
   if (rbuf.IRESPONSE.FINEOFFSETenabled) YADOMS_LOG(info) << "   - FineOffset/Viking";
   if (rbuf.IRESPONSE.LIGHTING4enabled ) YADOMS_LOG(info) << "   - Lighting4";
   if (rbuf.IRESPONSE.RSLenabled       ) YADOMS_LOG(info) << "   - RSL";
   if (rbuf.IRESPONSE.SXenabled        ) YADOMS_LOG(info) << "   - Byron SX";
   if (rbuf.IRESPONSE.RFU6enabled      ) YADOMS_LOG(info) << "   - RFU";
   if (rbuf.IRESPONSE.UNDECODEDenabled ) YADOMS_LOG(info) << "   - undecoded messages";

   if (rbuf.IRESPONSE.MERTIKenabled    ) YADOMS_LOG(info) << "   - Mertik";
   if (rbuf.IRESPONSE.LWRFenabled      ) YADOMS_LOG(info) << "   - AD LightwaveRF";
   if (rbuf.IRESPONSE.HIDEKIenabled    ) YADOMS_LOG(info) << "   - Hideki/UPM";
   if (rbuf.IRESPONSE.LACROSSEenabled  ) YADOMS_LOG(info) << "   - La Crosse";
   if (rbuf.IRESPONSE.FS20enabled      ) YADOMS_LOG(info) << "   - FS20";
   if (rbuf.IRESPONSE.PROGUARDenabled  ) YADOMS_LOG(info) << "   - ProGuard";
   if (rbuf.IRESPONSE.BLINDST0enabled  ) YADOMS_LOG(info) << "   - BlindsT0";
   if (rbuf.IRESPONSE.BLINDST1enabled  ) YADOMS_LOG(info) << "   - BlindsT1";

   if (rbuf.IRESPONSE.X10enabled       ) YADOMS_LOG(info) << "   - X10";
   if (rbuf.IRESPONSE.ARCenabled       ) YADOMS_LOG(info) << "   - ARC";
   if (rbuf.IRESPONSE.ACenabled        ) YADOMS_LOG(info) << "   - AC";
   if (rbuf.IRESPONSE.HEEUenabled      ) YADOMS_LOG(info) << "   - HomeEasy EU";
   if (rbuf.IRESPONSE.MEIANTECHenabled ) YADOMS_LOG(info) << "   - Meiantech";
   if (rbuf.IRESPONSE.OREGONenabled    ) YADOMS_LOG(info) << "   - Oregon Scientific";
   if (rbuf.IRESPONSE.ATIenabled       ) YADOMS_LOG(info) << "   - ATI";
   if (rbuf.IRESPONSE.VISONICenabled   ) YADOMS_LOG(info) << "   - Visonic";
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
      // TODO à compléter
   default:
      {
         YADOMS_LOG(error) << "Invalid RfxCom message received, unknown packet type " << std::setfill('0') << std::setw(sizeof(unsigned char) * 2) << std::hex << (int)buf->RXRESPONSE.packettype;
         return;
      }
   }
   sensorMessage->historizeData(context);
}