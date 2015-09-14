#include "stdafx.h"
#include <shared/Log.h>
#include "TransceiverStatus.h"
#include <shared/StringExtension.h>


namespace rfxcomMessages
{

CTransceiverStatus::CTransceiverStatus(const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeInterfaceMessage, sTypeInterfaceResponse, GET_RBUF_STRUCT_SIZE(IRESPONSE), seqNumberProvider->last());

   switch (rbuf.IRESPONSE.subtype)
   {
   case sTypeInterfaceResponse      : m_statusType = kStatus;                     break;
   case sTypeUnknownRFYremote       : m_statusType = kUnknownRfyRemote;           break;
   case sTypeExtError               : m_statusType = kNoExtendedHardwarePresent;  break;
   case sTypeRFYremoteList          : m_statusType = kListRfyMode;                break;
   case sTypeInterfaceWrongCommand  : m_statusType = kWrongCommand;               break;
   default:
      YADOMS_LOG(error) << "Unknown status subtype value : " << rbuf.IRESPONSE.subtype;
      break;
   }

   m_rfxcomType = rbuf.IRESPONSE.msg1;

   m_firmwareVersion = rbuf.IRESPONSE.msg2;

   // Enabled protocols
   m_AEenabled         = rbuf.IRESPONSE.AEenabled        ;
   m_RUBICSONenabled   = rbuf.IRESPONSE.RUBICSONenabled  ;
   m_FINEOFFSETenabled = rbuf.IRESPONSE.FINEOFFSETenabled;
   m_LIGHTING4enabled  = rbuf.IRESPONSE.LIGHTING4enabled ;
   m_RSLenabled        = rbuf.IRESPONSE.RSLenabled       ;
   m_SXenabled         = rbuf.IRESPONSE.SXenabled        ;
   m_RFU6enabled       = rbuf.IRESPONSE.RFU6enabled      ;
   m_UNDECODEDenabled  = rbuf.IRESPONSE.UNDECODEDenabled ;
   m_MERTIKenabled     = rbuf.IRESPONSE.MERTIKenabled    ;
   m_LWRFenabled       = rbuf.IRESPONSE.LWRFenabled      ;
   m_HIDEKIenabled     = rbuf.IRESPONSE.HIDEKIenabled    ;
   m_LACROSSEenabled   = rbuf.IRESPONSE.LACROSSEenabled  ;
   m_FS20enabled       = rbuf.IRESPONSE.FS20enabled      ;
   m_PROGUARDenabled   = rbuf.IRESPONSE.PROGUARDenabled  ;
   m_BLINDST0enabled   = rbuf.IRESPONSE.BLINDST0enabled  ;
   m_BLINDST1enabled   = rbuf.IRESPONSE.BLINDST1enabled  ;
   m_X10enabled        = rbuf.IRESPONSE.X10enabled       ;
   m_ARCenabled        = rbuf.IRESPONSE.ARCenabled       ;
   m_ACenabled         = rbuf.IRESPONSE.ACenabled        ;
   m_HEEUenabled       = rbuf.IRESPONSE.HEEUenabled      ;
   m_MEIANTECHenabled  = rbuf.IRESPONSE.MEIANTECHenabled ;
   m_OREGONenabled     = rbuf.IRESPONSE.OREGONenabled    ;
   m_ATIenabled        = rbuf.IRESPONSE.ATIenabled       ;
   m_VISONICenabled    = rbuf.IRESPONSE.VISONICenabled   ;
}                         

CTransceiverStatus::~CTransceiverStatus()
{
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CTransceiverStatus::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Status is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Status is a read-only message, can not be encoded");
}

void CTransceiverStatus::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   // Nothing to historize
}

const std::string& CTransceiverStatus::getDeviceName() const
{
   return shared::CStringExtension::EmptyString;
} 

void CTransceiverStatus::traceEnabledProtocols() const
{
   YADOMS_LOG(information) << "RFXCom configured protocols :";

   if (m_AEenabled        ) YADOMS_LOG(information) << "   - AE Blyss";
   if (m_RUBICSONenabled  ) YADOMS_LOG(information) << "   - Rubicson";
   if (m_FINEOFFSETenabled) YADOMS_LOG(information) << "   - FineOffset/Viking";
   if (m_LIGHTING4enabled ) YADOMS_LOG(information) << "   - Lighting4";
   if (m_RSLenabled       ) YADOMS_LOG(information) << "   - RSL";
   if (m_SXenabled        ) YADOMS_LOG(information) << "   - Byron SX";
   if (m_RFU6enabled      ) YADOMS_LOG(information) << "   - RFU";
   if (m_UNDECODEDenabled ) YADOMS_LOG(information) << "   - undecoded messages";

   if (m_MERTIKenabled    ) YADOMS_LOG(information) << "   - Mertik";
   if (m_LWRFenabled      ) YADOMS_LOG(information) << "   - AD LightwaveRF";
   if (m_HIDEKIenabled    ) YADOMS_LOG(information) << "   - Hideki/UPM";
   if (m_LACROSSEenabled  ) YADOMS_LOG(information) << "   - La Crosse";
   if (m_FS20enabled      ) YADOMS_LOG(information) << "   - FS20";
   if (m_PROGUARDenabled  ) YADOMS_LOG(information) << "   - ProGuard";
   if (m_BLINDST0enabled  ) YADOMS_LOG(information) << "   - BlindsT0";
   if (m_BLINDST1enabled  ) YADOMS_LOG(information) << "   - BlindsT1";

   if (m_X10enabled       ) YADOMS_LOG(information) << "   - X10";
   if (m_ARCenabled       ) YADOMS_LOG(information) << "   - ARC";
   if (m_ACenabled        ) YADOMS_LOG(information) << "   - AC";
   if (m_HEEUenabled      ) YADOMS_LOG(information) << "   - HomeEasy EU";
   if (m_MEIANTECHenabled ) YADOMS_LOG(information) << "   - Meiantech";
   if (m_OREGONenabled    ) YADOMS_LOG(information) << "   - Oregon Scientific";
   if (m_ATIenabled       ) YADOMS_LOG(information) << "   - ATI";
   if (m_VISONICenabled   ) YADOMS_LOG(information) << "   - Visonic";
}

CTransceiverStatus::EStatusType CTransceiverStatus::getStatusType() const
{
   return m_statusType;
}

unsigned char CTransceiverStatus::getRfxcomType() const
{
   return m_rfxcomType;
}

const std::string CTransceiverStatus::rfxcomTypeToString() const
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

   std::map<unsigned char, std::string>::const_iterator itRfxcomTypes = RfxcomTypes.find(m_rfxcomType);
   if (itRfxcomTypes == RfxcomTypes.end())
      return boost::lexical_cast<std::string>(m_rfxcomType);

   return itRfxcomTypes->second;
}

unsigned int CTransceiverStatus::getFirmwareVersion() const
{
   return m_firmwareVersion;
}

bool CTransceiverStatus::needConfigurationUpdate(const IRfxcomConfiguration& configuration) const
{
   if (
      (configuration.isAEenabled        () == m_AEenabled        ) &&
      (configuration.isRUBICSONenabled  () == m_RUBICSONenabled  ) &&
      (configuration.isFINEOFFSETenabled() == m_FINEOFFSETenabled) &&
      (configuration.isLIGHTING4enabled () == m_LIGHTING4enabled ) &&
      (configuration.isRSLenabled       () == m_RSLenabled       ) &&
      (configuration.isSXenabled        () == m_SXenabled        ) &&
      (configuration.isRFU6enabled      () == m_RFU6enabled      ) &&
      (configuration.isUNDECODEDenabled () == m_UNDECODEDenabled ) &&
      (configuration.isMERTIKenabled    () == m_MERTIKenabled    ) &&
      (configuration.isLWRFenabled      () == m_LWRFenabled      ) &&
      (configuration.isHIDEKIenabled    () == m_HIDEKIenabled    ) &&
      (configuration.isLACROSSEenabled  () == m_LACROSSEenabled  ) &&
      (configuration.isFS20enabled      () == m_FS20enabled      ) &&
      (configuration.isPROGUARDenabled  () == m_PROGUARDenabled  ) &&
      (configuration.isBLINDST0enabled  () == m_BLINDST0enabled  ) &&
      (configuration.isBLINDST1enabled  () == m_BLINDST1enabled  ) &&
      (configuration.isX10enabled       () == m_X10enabled       ) &&
      (configuration.isARCenabled       () == m_ARCenabled       ) &&
      (configuration.isACenabled        () == m_ACenabled        ) &&
      (configuration.isHEEUenabled      () == m_HEEUenabled      ) &&
      (configuration.isMEIANTECHenabled () == m_MEIANTECHenabled ) &&
      (configuration.isOREGONenabled    () == m_OREGONenabled    ) &&
      (configuration.isATIenabled       () == m_ATIenabled       ) &&
      (configuration.isVISONICenabled   () == m_VISONICenabled   )
      )
      return false;

   return true;
}

} // namespace rfxcomMessages