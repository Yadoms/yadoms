#include "stdafx.h"
#include "TransceiverStatus.h"


namespace rfxcomMessages
{
   DECLARE_ENUM_IMPLEMENTATION_NESTED(CTransceiverStatus::EFirmwareType, EFirmwareType,
      ((Type1ReceiveOnly))
      ((Type1))
      ((Type2))
      ((Ext))
      ((Ext2))
   );

   CTransceiverStatus::CTransceiverStatus(const RBUF& rbuf,
                                          size_t rbufSize,
                                          boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   {
      // This message is length variable, so manage all cases :
      if (rbufSize < (GET_RBUF_STRUCT_SIZE(RXRESPONSE) - 1))
      {
         // Message is too small
         throw shared::exception::CException((boost::format("Wrong message length, received : %1%, expected : %2% (total_message_size - 1)") % rbufSize % (GET_RBUF_STRUCT_SIZE(RXRESPONSE) - 1)).str());
      }

      if (rbuf.RXRESPONSE.packettype == sTypeRecStarted)
      {
         // Length is specific for the receiver started message
         CheckReceivedMessage(rbuf,
                              rbufSize,
                              pTypeInterfaceMessage,
                              sTypeRecStarted,
                              GET_RBUF_STRUCT_SIZE(IRESPONSE),
                              seqNumberProvider->last());
      }
      else
      {
         // Length is always 0x0D (see RFXCom SDK specification) for all subtype messages
         CheckReceivedMessage(rbuf,
                              rbufSize,
                              pTypeInterfaceMessage,
                              DONT_CHECK_SUBTYPE,
                              DONT_CHECK_SIZE,
                              seqNumberProvider->last());
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
         std::cerr << "Unknown status subtype value : " << rbuf.IRESPONSE.subtype << std::endl;
         break;
      }

      if (m_statusType == kReceiverStarted)
      {
         m_validMessage = std::string(reinterpret_cast<const char*>(&rbuf.IRESPONSE.msg1), 16);
      }
      else
      {
         // Get the Firmware type (code from RFXManager application)
         m_rfxcomType = rbuf.IRESPONSE.msg1;

         if (rbuf.IRESPONSE.packetlength > 13)
         {
            // New IRESPONSE message format
            m_firmwareType = rbuf.IRESPONSE.msg10;
            m_firmwareVersion = rbuf.IRESPONSE.msg2 + 1000;
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
         m_FS20enabled = rbuf.IRESPONSE.FS20enabled;
         m_PROGUARDenabled = rbuf.IRESPONSE.PROGUARDenabled;
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

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CTransceiverStatus::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Status is a read-only message, can not be encoded");
   }

   void CTransceiverStatus::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize
   }

   const std::string& CTransceiverStatus::getDeviceName() const
   {
      static const std::string emptyString;
      return emptyString;
   }

   void CTransceiverStatus::traceEnabledProtocols() const
   {
      std::cout << "RFXCom configured protocols :" << std::endl;

      if (m_AEenabled) std::cout << "   - AE Blyss" << std::endl;
      if (m_RUBICSONenabled) std::cout << "   - Rubicson" << std::endl;
      if (m_FINEOFFSETenabled) std::cout << "   - FineOffset/Viking" << std::endl;
      if (m_LIGHTING4enabled) std::cout << "   - Lighting4" << std::endl;
      if (m_RSLenabled) std::cout << "   - RSL" << std::endl;
      if (m_SXenabled) std::cout << "   - Byron SX" << std::endl;
      if (m_IMAGINTRONIXenabled) std::cout << "   - Imagintronix/Opus" << std::endl;
      if (m_UNDECODEDenabled) std::cout << "   - undecoded messages" << std::endl;

      if (m_MERTIKenabled) std::cout << "   - Mertik" << std::endl;
      if (m_LWRFenabled) std::cout << "   - AD LightwaveRF" << std::endl;
      if (m_HIDEKIenabled) std::cout << "   - Hideki/UPM" << std::endl;
      if (m_LACROSSEenabled) std::cout << "   - La Crosse" << std::endl;
      if (m_FS20enabled) std::cout << "   - FS20" << std::endl;
      if (m_PROGUARDenabled) std::cout << "   - ProGuard" << std::endl;
      if (m_BLINDST0enabled) std::cout << "   - BlindsT0" << std::endl;
      if (m_BLINDST1enabled) std::cout << "   - BlindsT1" << std::endl;

      if (m_X10enabled) std::cout << "   - X10" << std::endl;
      if (m_ARCenabled) std::cout << "   - ARC" << std::endl;
      if (m_ACenabled) std::cout << "   - AC" << std::endl;
      if (m_HEEUenabled) std::cout << "   - HomeEasy EU" << std::endl;
      if (m_MEIANTECHenabled) std::cout << "   - Meiantech" << std::endl;
      if (m_OREGONenabled) std::cout << "   - Oregon Scientific" << std::endl;
      if (m_ATIenabled) std::cout << "   - ATI" << std::endl;
      if (m_VISONICenabled) std::cout << "   - Visonic" << std::endl;

      if (m_KeeLoqenabled) std::cout << "   - KeeLoq" << std::endl;
      if (m_HomeConfortenabled) std::cout << "   - HomeConfort" << std::endl;
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
      static const std::map<unsigned char, std::string> RfxcomTypes = boost::assign::map_list_of
      (recType310, "310MHz")
         (recType315, "315MHz")
         (recType43392, "433.92MHz receiver only")
         (trxType43392, "433.92MHz transceiver")
         (0x54, "433.42MHz") // No constant is defined in rfxtrx.h v6.19
         (recType86800, "868.00MHz")
         (recType86800FSK, "868.00MHz FSK")
         (recType86830, "868.30MHz")
         (recType86830FSK, "868.30MHz FSK")
         (recType86835, "868.35MHz")
         (recType86835FSK, "868.35MHz FSK")
         (recType86895, "868.95MHz");

      std::map<unsigned char, std::string>::const_iterator itRfxcomTypes = RfxcomTypes.find(m_rfxcomType);
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
         (configuration.isAEenabled() == m_AEenabled) &&
         (configuration.isRUBICSONenabled() == m_RUBICSONenabled) &&
         (configuration.isFINEOFFSETenabled() == m_FINEOFFSETenabled) &&
         (configuration.isLIGHTING4enabled() == m_LIGHTING4enabled) &&
         (configuration.isRSLenabled() == m_RSLenabled) &&
         (configuration.isSXenabled() == m_SXenabled) &&
         (configuration.isIMAGINTRONIXenabled() == m_IMAGINTRONIXenabled) &&
         (configuration.isUNDECODEDenabled() == m_UNDECODEDenabled) &&
         (configuration.isMERTIKenabled() == m_MERTIKenabled) &&
         (configuration.isLWRFenabled() == m_LWRFenabled) &&
         (configuration.isHIDEKIenabled() == m_HIDEKIenabled) &&
         (configuration.isLACROSSEenabled() == m_LACROSSEenabled) &&
         (configuration.isFS20enabled() == m_FS20enabled) &&
         (configuration.isPROGUARDenabled() == m_PROGUARDenabled) &&
         (configuration.isBLINDST0enabled() == m_BLINDST0enabled) &&
         (configuration.isBLINDST1enabled() == m_BLINDST1enabled) &&
         (configuration.isX10enabled() == m_X10enabled) &&
         (configuration.isARCenabled() == m_ARCenabled) &&
         (configuration.isACenabled() == m_ACenabled) &&
         (configuration.isHEEUenabled() == m_HEEUenabled) &&
         (configuration.isMEIANTECHenabled() == m_MEIANTECHenabled) &&
         (configuration.isOREGONenabled() == m_OREGONenabled) &&
         (configuration.isATIenabled() == m_ATIenabled) &&
         (configuration.isVISONICenabled() == m_VISONICenabled) &&
         (configuration.isKeeLoqenabled() == m_KeeLoqenabled) &&
         (configuration.isHomeConfortenabled() == m_HomeConfortenabled)
         )
         return false;

      return true;
   }
} // namespace rfxcomMessages


