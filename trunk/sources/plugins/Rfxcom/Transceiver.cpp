#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/serialization/PTreeToJsonSerializer.h>
#include <shared/exception/InvalidParameter.hpp>
#include "rfxcomMessages/Lighting1.h"
#include "rfxcomMessages/Lighting3.h"
#include "rfxcomMessages/Lighting6.h"
#include "rfxcomMessages/Curtain1.h"
#include "NullSequenceNumber.h"
#include "PortException.hpp"


// Macro helpers to access RBUF parts
// - Get buffer pointer and size
#define BUFFER_AND_SIZE(structure) &structure,sizeof(structure)
// - to clear a part of RBUF
#define MEMCLEAR(structure) memset(&structure,0,sizeof(structure));
// - Log
#define LOG_WRITE(structure) YADOMS_LOG(debug)<<" RFXCom <<< "<<msgToString(BUFFER_AND_SIZE(structure));
#define LOG_READ(structure) YADOMS_LOG(debug)<<" RFXCom >>> "<<msgToString(BUFFER_AND_SIZE(structure));


CTransceiver::CTransceiver(boost::shared_ptr<IPort> port)
   :m_port(port), m_seqNumberProvider(new CNullSequenceNumber())
{
   MEMCLEAR(m_request);
   MEMCLEAR(m_answer);
}

CTransceiver::~CTransceiver()
{
}

void CTransceiver::sendReset()
{
   // Reset the RFXCom.
   // See the RFXCom SDK specification for more information about this sequence

   try
   {
      sendCommand(cmdRESET);
      boost::this_thread::sleep(boost::posix_time::milliseconds(50));
      m_port->flush();
      sendCommand(cmdSTATUS);
      waitStatus();
   }
   catch (CPortException& e)
   {
      YADOMS_LOG(error) << "Error resetting RFXCom transceiver : " << e.what();
   }
}

void CTransceiver::sendCommand(unsigned char command)
{
   MEMCLEAR(m_request.ICMND);

   m_request.ICMND.packetlength = sizeof(m_request.ICMND);
   m_request.ICMND.packettype = pTypeInterfaceControl;
   m_request.ICMND.subtype = sTypeInterfaceCommand;
   m_request.ICMND.seqnbr = m_seqNumberProvider->getNext();
   m_request.ICMND.cmnd = command;

   LOG_WRITE(m_request.ICMND);

   m_port->send(boost::asio::const_buffer(BUFFER_AND_SIZE(m_request.ICMND)));
}

bool CTransceiver::waitStatus()
{
   boost::asio::mutable_buffer buffer;
   m_port->receive(buffer);

   // Check answer

   // - Check message size
   if (boost::asio::buffer_size(buffer) != sizeof(m_answer.IRESPONSE))  // TODO Seule solution pour obtenir la taille de RBUF::IRESPONSE. Si m_answer n'est pas nécessaire, utiliser une static const locale
   {
      YADOMS_LOG(error) << "Error reading status, invalid message size";
      return false;
   }

   const RBUF* answer = boost::asio::buffer_cast<const RBUF*>(buffer);

   // - Check message data
   if (answer->IRESPONSE.packetlength != (sizeof(m_answer.IRESPONSE) - 1) ||
      answer->IRESPONSE.packettype != pTypeInterfaceMessage ||
      answer->IRESPONSE.subtype != sTypeInterfaceResponse ||
      answer->IRESPONSE.seqnbr != m_request.ICMND.seqnbr ||
      answer->IRESPONSE.cmnd != cmdSTATUS)
   {
      YADOMS_LOG(error) << "Error reading status, invalid data received";
      return false;
   }

   YADOMS_LOG(info) << "RFXCom status, type (" << rfxcomTypeToString(answer->IRESPONSE.msg1) << "), firmware version (" << answer->IRESPONSE.msg2 << ")";

   TraceRfxComConfiguredProtocols(*answer);

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
      (recType86895     , "868.95MHz"              );   std::map<unsigned char, std::string>::const_iterator itRfxcomTypes = RfxcomTypes.find(rfxcomType);   if (itRfxcomTypes == RfxcomTypes.end())      return boost::lexical_cast<std::string>(rfxcomType);   return itRfxcomTypes->second;}

void CTransceiver::TraceRfxComConfiguredProtocols(const RBUF& rbuf)
{
   YADOMS_LOG(info) << "RFXCom configured protocols :";

   if (rbuf.IRESPONSE.AEenabled        ) YADOMS_LOG(info) << "   - AE Blyss";   if (rbuf.IRESPONSE.RUBICSONenabled  ) YADOMS_LOG(info) << "   - Rubicson";   if (rbuf.IRESPONSE.FINEOFFSETenabled) YADOMS_LOG(info) << "   - FineOffset/Viking";   if (rbuf.IRESPONSE.LIGHTING4enabled ) YADOMS_LOG(info) << "   - Lighting4";   if (rbuf.IRESPONSE.RSLenabled       ) YADOMS_LOG(info) << "   - RSL";   if (rbuf.IRESPONSE.SXenabled        ) YADOMS_LOG(info) << "   - Byron SX";   if (rbuf.IRESPONSE.RFU6enabled      ) YADOMS_LOG(info) << "   - RFU";   if (rbuf.IRESPONSE.UNDECODEDenabled ) YADOMS_LOG(info) << "   - undecoded messages";   if (rbuf.IRESPONSE.MERTIKenabled    ) YADOMS_LOG(info) << "   - Mertik";   if (rbuf.IRESPONSE.LWRFenabled      ) YADOMS_LOG(info) << "   - AD LightwaveRF";   if (rbuf.IRESPONSE.HIDEKIenabled    ) YADOMS_LOG(info) << "   - Hideki/UPM";   if (rbuf.IRESPONSE.LACROSSEenabled  ) YADOMS_LOG(info) << "   - La Crosse";   if (rbuf.IRESPONSE.FS20enabled      ) YADOMS_LOG(info) << "   - FS20";   if (rbuf.IRESPONSE.PROGUARDenabled  ) YADOMS_LOG(info) << "   - ProGuard";   if (rbuf.IRESPONSE.BLINDST0enabled  ) YADOMS_LOG(info) << "   - BlindsT0";   if (rbuf.IRESPONSE.BLINDST1enabled  ) YADOMS_LOG(info) << "   - BlindsT1";   if (rbuf.IRESPONSE.X10enabled       ) YADOMS_LOG(info) << "   - X10";   if (rbuf.IRESPONSE.ARCenabled       ) YADOMS_LOG(info) << "   - ARC";   if (rbuf.IRESPONSE.ACenabled        ) YADOMS_LOG(info) << "   - AC";   if (rbuf.IRESPONSE.HEEUenabled      ) YADOMS_LOG(info) << "   - HomeEasy EU";   if (rbuf.IRESPONSE.MEIANTECHenabled ) YADOMS_LOG(info) << "   - Meiantech";   if (rbuf.IRESPONSE.OREGONenabled    ) YADOMS_LOG(info) << "   - Oregon Scientific";   if (rbuf.IRESPONSE.ATIenabled       ) YADOMS_LOG(info) << "   - ATI";   if (rbuf.IRESPONSE.VISONICenabled   ) YADOMS_LOG(info) << "   - Visonic";}


std::string CTransceiver::msgToString(const void* ptr, size_t size) const
{
   std::ostringstream ss;
   const unsigned char* ucharPtr = reinterpret_cast<const unsigned char*> (ptr);
   for (size_t idx = 0 ; idx < size ; ++ idx)
      ss << std::fixed << std::setprecision(2) << std::hex << ucharPtr[idx] << " ";

   return ss.str();
}

void CTransceiver::send(boost::shared_ptr<yApi::IDeviceCommand> command)
{
   try
   {
      boost::shared_ptr<rfxcomMessages::IRfxcomMessage> rfxcomMsg = createRfxcomMessage(command);

      m_port->send(rfxcomMsg->getBuffer());
   }
   catch (shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Error sending command to RFXCom : " << e.what();
      BOOST_ASSERT_MSG(false, "Error sending command to RFXCom");
   }
}

boost::shared_ptr<rfxcomMessages::IRfxcomMessage> CTransceiver::createRfxcomMessage(boost::shared_ptr<yApi::IDeviceCommand> command) const
{
   shared::serialization::CPtreeToJsonSerializer serializer;
   try
   {
      boost::property_tree::ptree commandBody = serializer.deserialize(command->getBody());
      unsigned char type = commandBody.get<unsigned char>("type");
      boost::shared_ptr<rfxcomMessages::IRfxcomMessage> rfxcomMsg;

      // Create the RFXCom message
      switch(type)
      {
      case pTypeLighting1:
         rfxcomMsg.reset(new rfxcomMessages::CLighting1(commandBody, m_seqNumberProvider));
         break;
      case pTypeLighting3:
         rfxcomMsg.reset(new rfxcomMessages::CLighting3(commandBody, m_seqNumberProvider));
         break;
      case pTypeLighting6:
         rfxcomMsg.reset(new rfxcomMessages::CLighting6(commandBody, m_seqNumberProvider));
         break;
      case pTypeCurtain:
         rfxcomMsg.reset(new rfxcomMessages::CCurtain1(commandBody, m_seqNumberProvider));
         break;
         //TODO compléter
      default:
         YADOMS_LOG(error) << "Invalid command \"" << command->toString() << "\" : " << " unknown type " << type;
         BOOST_ASSERT_MSG(false, "Invalid command (unknown type)");
         break;
      }

      return rfxcomMsg;
   }
   catch (boost::property_tree::ptree_bad_path& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid command (parameter doesn't exist)");
      throw shared::exception::CInvalidParameter("Invalid command \"" + command->toString() + "\" : " + e.what());
   }
   catch (boost::property_tree::ptree_bad_data& e)
   {
      BOOST_ASSERT_MSG(false, "Invalid command (fail to extract parameter)");
      throw shared::exception::CInvalidParameter("Invalid command \"" + command->toString() + "\" : " + e.what());
   }
}