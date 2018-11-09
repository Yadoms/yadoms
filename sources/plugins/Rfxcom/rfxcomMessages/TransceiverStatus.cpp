#include "stdafx.h"
#include "TransceiverStatus.h"
#include <shared/Log.h>


namespace rfxcomMessages
{
   DECLARE_ENUM_IMPLEMENTATION_NESTED(CTransceiverStatus::EFirmwareType, EFirmwareType,
      ((Type1ReceiveOnly))
      ((Type1))
      ((Type2))
      ((Ext))
      ((Ext2))
      ((Pro1))
      ((Pro2))
      ((ProXL1))
   );

   CTransceiverStatus::CTransceiverStatus(const RBUF& rbuf,
                                          size_t rbufSize,
                                          boost::shared_ptr<ISequenceNumber> seqNumberProvider)
   {
      // This message is length variable, so manage all cases :
      if (rbufSize < (GET_RBUF_STRUCT_SIZE(RXRESPONSE) - 1))
      {
         // Message is too small
         throw shared::exception::CException(
            (boost::format("Wrong message length, received : %1%, expected : %2% (total_message_size - 1)") % rbufSize % (GET_RBUF_STRUCT_SIZE(
               RXRESPONSE) - 1)).str());
      }

      if (rbuf.RXRESPONSE.subtype == sTypeRecStarted)
      {
         // Length is specific for the receiver started message
         CheckReceivedMessage(rbuf,
                              rbufSize,
                              pTypeInterfaceMessage,
                              sTypeRecStarted,
                              GET_RBUF_STRUCT_SIZE(IRESPONSE),
                              seqNumberProvider);
      }
      else
      {
         // Length is always 0x0D (see RFXCom SDK specification) for all subtype messages
         CheckReceivedMessage(rbuf,
                              rbufSize,
                              pTypeInterfaceMessage,
                              DONT_CHECK_SUBTYPE,
                              DONT_CHECK_SIZE,
                              seqNumberProvider);
      }

      switch (rbuf.IRESPONSE.subtype)
      {
      case sTypeInterfaceResponse: m_statusType = kStatus;
         break;
      case sTypeUnknownRFYremote: m_statusType = kUnknownRfyRemote;
         break;
      case sTypeExtError: m_statusType = kNoExtendedHardwarePresent;
         break;
      case sTypeRFYremoteList: m_statusType = kListRfyMode;
         break;
      case sTypeASAremoteList: m_statusType = kListAsaMode;
         break;
      case sTypeRecStarted: m_statusType = kReceiverStarted;
         break;
      case sTypeInterfaceWrongCommand: m_statusType = kWrongCommand;
         break;
      default:
         throw shared::exception::CException((boost::format("Unknown status subtype value : %1%") % rbuf.IRESPONSE.subtype).str());
      }

      if (m_statusType == kReceiverStarted)
      {
         m_validMessage = reinterpret_cast<const char*>(&rbuf.IRESPONSE.msg1);
      }
      else
      {
         // Get the Firmware type (code from RFXManager application)
         m_rfxcomType = rbuf.IRESPONSE.msg1;

         if (rbuf.IRESPONSE.packetlength > 13)
         {
            // New IRESPONSE message format
            switch (rbuf.IRESPONSE.msg10)
            {
            case FWtyperec:
               m_firmwareType = EFirmwareType::kType1ReceiveOnly;
               break;
            case FWtype1:
               m_firmwareType = EFirmwareType::kType1;
               break;
            case FWtype2:
               m_firmwareType = EFirmwareType::kType2;
               break;
            case FWtypeExt:
               m_firmwareType = EFirmwareType::kExt;
               break;
            case FWtypeExt2:
               m_firmwareType = EFirmwareType::kExt2;
               break;
            case FWtypePro1:
               m_firmwareType = EFirmwareType::kPro1;
               break;
            case FWtypePro2:
               m_firmwareType = EFirmwareType::kPro2;
               break;
            case FWtypeProXL1:
               m_firmwareType = EFirmwareType::kProXL1;
               break;
            default:
               throw shared::exception::CException((boost::format("Unknown firmware type value : %1%") % rbuf.IRESPONSE.msg10).str());
            }
            m_firmwareVersion = rbuf.IRESPONSE.msg2 + 1000;
            m_hardwareVersionMajor = rbuf.IRESPONSE.msg7;
            m_hardwareVersionMinor = rbuf.IRESPONSE.msg8;
         }
         else
         {
            // Old IRESPONSE message format
            m_firmwareVersion = rbuf.IRESPONSE.msg2;
            if (m_rfxcomType == recType43392 && m_firmwareVersion < 162)
               m_firmwareType = EFirmwareType::kType1ReceiveOnly;
            else if (m_rfxcomType == trxType43392 && m_firmwareVersion < 162)
               m_firmwareType = EFirmwareType::kType1;
            else if (m_rfxcomType == trxType43392 && m_firmwareVersion >= 162 && m_firmwareVersion < 225)
               m_firmwareType = EFirmwareType::kType2;
            else
               m_firmwareType = EFirmwareType::kExt;

            m_hardwareVersionMajor = 0;
            m_hardwareVersionMinor = 0;
         }

         // Enabled protocols
         m_AEenabled = rbuf.IRESPONSE.AEenabled;
         m_RUBICSONenabled = rbuf.IRESPONSE.RUBICSONenabled;
         m_FINEOFFSETenabled = rbuf.IRESPONSE.FINEOFFSETenabled;
         m_LIGHTING4enabled = rbuf.IRESPONSE.LIGHTING4enabled;
         m_RSLenabled = rbuf.IRESPONSE.RSLenabled;
         m_SXenabled = rbuf.IRESPONSE.SXenabled;
         m_IMAGINTRONIXenabled = rbuf.IRESPONSE.IMAGINTRONIXenabled;
         m_UNDECODEDenabled = rbuf.IRESPONSE.UNDECODEDenabled;
         m_MERTIKenabled = rbuf.IRESPONSE.MERTIKenabled;
         m_LWRFenabled = rbuf.IRESPONSE.LWRFenabled;
         m_HIDEKIenabled = rbuf.IRESPONSE.HIDEKIenabled;
         m_LACROSSEenabled = rbuf.IRESPONSE.LACROSSEenabled;
         m_LegrandCADenabled = rbuf.IRESPONSE.LEGRANDenabled;
         m_BLINDST0enabled = rbuf.IRESPONSE.BLINDST0enabled;
         m_BLINDST1enabled = rbuf.IRESPONSE.BLINDST1enabled;
         m_X10enabled = rbuf.IRESPONSE.X10enabled;
         m_ARCenabled = rbuf.IRESPONSE.ARCenabled;
         m_ACenabled = rbuf.IRESPONSE.ACenabled;
         m_HEEUenabled = rbuf.IRESPONSE.HEEUenabled;
         m_MEIANTECHenabled = rbuf.IRESPONSE.MEIANTECHenabled;
         m_OREGONenabled = rbuf.IRESPONSE.OREGONenabled;
         m_ATIenabled = rbuf.IRESPONSE.ATIenabled;
         m_VISONICenabled = rbuf.IRESPONSE.VISONICenabled;
         m_KeeLoqenabled = rbuf.IRESPONSE.KEELOQenabled;
         m_HomeConfortenabled = rbuf.IRESPONSE.HCEnabled;
      }
   }

   CTransceiverStatus::~CTransceiverStatus()
   {
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CTransceiverStatus::encode(
      boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Status is a read-only message, can not be encoded");
   }

   void CTransceiverStatus::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize
   }

   void CTransceiverStatus::filter() const
   {
   }

   void CTransceiverStatus::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::string& CTransceiverStatus::getDeviceName() const
   {
      static const std::string emptyString;
      return emptyString;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CTransceiverStatus::keywords()
   {
      return NoKeywords;
   }

   void CTransceiverStatus::traceEnabledProtocols() const
   {
      YADOMS_LOG(information) << "RFXCom configured protocols :";

      if (m_AEenabled) YADOMS_LOG(information) << "   - AE Blyss";
      if (m_RUBICSONenabled) YADOMS_LOG(information) << "   - Rubicson";
      if (m_FINEOFFSETenabled) YADOMS_LOG(information) << "   - FineOffset, Viking";
      if (m_LIGHTING4enabled) YADOMS_LOG(information) << "   - Lighting4";
      if (m_RSLenabled) YADOMS_LOG(information) << "   - RSL, Revolt";
      if (m_SXenabled) YADOMS_LOG(information) << "   - Byron SX, SelectPlus";
      if (m_IMAGINTRONIXenabled) YADOMS_LOG(information) << "   - Imagintronix/Opus";
      if (m_UNDECODEDenabled) YADOMS_LOG(information) << "   - undecoded messages";

      if (m_MERTIKenabled) YADOMS_LOG(information) << "   - Mertik";
      if (m_LWRFenabled) YADOMS_LOG(information) << "   - AD LightwaveRF";
      if (m_HIDEKIenabled) YADOMS_LOG(information) << "   - Hideki, TFA, Cresta, UPM";
      if (m_LACROSSEenabled) YADOMS_LOG(information) << "   - La Crosse";
      if (m_LegrandCADenabled) YADOMS_LOG(information) << "   - Legrand CAD";
      if (m_BLINDST0enabled) YADOMS_LOG(information) << "   - BlindsT0";
      if (m_BLINDST1enabled) YADOMS_LOG(information) << "   - BlindsT1";

      if (m_X10enabled) YADOMS_LOG(information) << "   - X10";
      if (m_ARCenabled) YADOMS_LOG(information) << "   - ARC";
      if (m_ACenabled) YADOMS_LOG(information) << "   - AC";
      if (m_HEEUenabled) YADOMS_LOG(information) << "   - HomeEasy EU";
      if (m_MEIANTECHenabled) YADOMS_LOG(information) << "   - Meiantech";
      if (m_OREGONenabled) YADOMS_LOG(information) << "   - Oregon Scientific";
      if (m_ATIenabled) YADOMS_LOG(information) << "   - ATI";
      if (m_VISONICenabled) YADOMS_LOG(information) << "   - Visonic";

      if (m_KeeLoqenabled) YADOMS_LOG(information) << "   - KeeLoq";
      if (m_HomeConfortenabled) YADOMS_LOG(information) << "   - HomeConfort";
   }

   CTransceiverStatus::EStatusType CTransceiverStatus::getStatusType() const
   {
      return m_statusType;
   }

   unsigned char CTransceiverStatus::getRfxcomType() const
   {
      if (m_statusType == kReceiverStarted)
         throw std::out_of_range("RFXCom type not avaible in this message");

      return m_rfxcomType;
   }

   std::string CTransceiverStatus::rfxcomTypeToString() const
   {
      static const std::map<unsigned int, std::string> RfxcomTypes = boost::assign::map_list_of
         (trxType310, "RFXtrx315 operating at 310MHz")
         (trxType315, "RFXtrx315 operating at 315MHz")
         (recType43392, "RFXrec433 operating at 433.92MHz (receiver only)")
         (trxType43392, "RFXtrx433 operating at 433.92MHz")
         (0x54, "RFXtrx433 operating at 433.42MHz (internal use)") // No constant is defined in rfxtrx.h v9.17
         (trxType868, "RFXtrx868X operating at 868MHz");

      const auto itRfxcomTypes = RfxcomTypes.find(m_rfxcomType);
      if (itRfxcomTypes == RfxcomTypes.end())
         return boost::lexical_cast<std::string>(m_rfxcomType);

      return itRfxcomTypes->second;
   }

   CTransceiverStatus::EFirmwareType CTransceiverStatus::getFirmwareType() const
   {
      if (m_statusType == kReceiverStarted)
         throw std::out_of_range("RFXCom firmware type version not avaible in this message");

      return m_firmwareType;
   }

   unsigned int CTransceiverStatus::getFirmwareVersion() const
   {
      if (m_statusType == kReceiverStarted)
         throw std::out_of_range("RFXCom firmware version not avaible in this message");

      return m_firmwareVersion;
   }

   std::string CTransceiverStatus::getHardwareVersion() const
   {
      if (m_statusType == kReceiverStarted)
         throw std::out_of_range("RFXCom hardware version not avaible in this message");

      return (boost::format("%1%.%2%") % m_hardwareVersionMajor % m_hardwareVersionMinor).str();
   }

   std::string CTransceiverStatus::getValidMessage() const
   {
      if (m_statusType != kReceiverStarted)
         throw std::out_of_range("RFXCom valid message not available in this message");

      return m_validMessage;
   }

   bool CTransceiverStatus::needConfigurationUpdate(const IRfxcomConfiguration& configuration) const
   {
      if (m_statusType == kReceiverStarted)
         throw std::out_of_range("RFXCom configuration not available in this message");

      if (
         configuration.isAEenabled() == m_AEenabled &&
         configuration.isRUBICSONenabled() == m_RUBICSONenabled &&
         configuration.isFINEOFFSETenabled() == m_FINEOFFSETenabled &&
         configuration.isLIGHTING4enabled() == m_LIGHTING4enabled &&
         configuration.isRSLenabled() == m_RSLenabled &&
         configuration.isSXenabled() == m_SXenabled &&
         configuration.isIMAGINTRONIXenabled() == m_IMAGINTRONIXenabled &&
         configuration.isUNDECODEDenabled() == m_UNDECODEDenabled &&
         configuration.isMERTIKenabled() == m_MERTIKenabled &&
         configuration.isLWRFenabled() == m_LWRFenabled &&
         configuration.isHIDEKIenabled() == m_HIDEKIenabled &&
         configuration.isLACROSSEenabled() == m_LACROSSEenabled &&
         configuration.isLEGRANDenabled() == m_LegrandCADenabled &&
         configuration.isBLINDST0enabled() == m_BLINDST0enabled &&
         configuration.isBLINDST1enabled() == m_BLINDST1enabled &&
         configuration.isX10enabled() == m_X10enabled &&
         configuration.isARCenabled() == m_ARCenabled &&
         configuration.isACenabled() == m_ACenabled &&
         configuration.isHEEUenabled() == m_HEEUenabled &&
         configuration.isMEIANTECHenabled() == m_MEIANTECHenabled &&
         configuration.isOREGONenabled() == m_OREGONenabled &&
         configuration.isATIenabled() == m_ATIenabled &&
         configuration.isVISONICenabled() == m_VISONICenabled &&
         configuration.isKeeLoqenabled() == m_KeeLoqenabled &&
         configuration.isHomeConfortenabled() == m_HomeConfortenabled
      )
         return false;

      return true;
   }
} // namespace rfxcomMessages
