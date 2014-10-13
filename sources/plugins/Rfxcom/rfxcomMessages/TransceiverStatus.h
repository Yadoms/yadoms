#pragma once
#include "RFXtrxHelpers.h"
#include "IRfxcomMessage.h"
#include "../IRfxcomConfiguration.h"

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	This class manage the RFXCom status
   //--------------------------------------------------------------
   class CTransceiverStatus : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief  The status type
      //--------------------------------------------------------------
      enum  EStatusType
      {
         kStatus,                      /// Response on a mode command
         kUnknownRfyRemote,            /// Unknown RFY remote
         kNoExtendedHardwarePresent,   /// No extended hardware present
         kListRfyMode,                 /// List RFY remotes
         kWrongCommand                 /// Wrong command received from the application
      };

   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] seqNumberProvider       The sequence number provider
      /// \param[in] rbuf                    Status received from Rfxcom
      //--------------------------------------------------------------
      CTransceiverStatus(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CTransceiverStatus();

      // IRfxcomMessage implementation
      virtual const shared::communication::CByteBuffer encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const;
      // [END] IRfxcomMessage implementation

      //--------------------------------------------------------------
      /// \brief	                     Trace the RFXCom configured protocols
      //--------------------------------------------------------------
      void traceEnabledProtocols() const;

      //--------------------------------------------------------------
      /// \brief	                     Get the status type (= message subType)
      // \return                       The status type
      //--------------------------------------------------------------
      EStatusType getStatusType() const;

      //--------------------------------------------------------------
      /// \brief	                     Get the RfxCom type
      // \return                       The RfxCom type
      //--------------------------------------------------------------
      unsigned char getRfxcomType() const;

      //--------------------------------------------------------------
      /// \brief	                     Get the RfxCom type as string
      // \return                       A string representing the RfxCom type
      //--------------------------------------------------------------
      const std::string rfxcomTypeToString() const;

      //--------------------------------------------------------------
      /// \brief	                     Get the RfxCom type as string
      // \return                       A string representing the RfxCom type
      //--------------------------------------------------------------
      unsigned int getFirmwareVersion() const;

      //--------------------------------------------------------------
      /// \brief	                     Check if RfxCom configuration must be updated (need protocols enabled for example)
      /// \param[in] configuration     The configured RfxCom status
      /// \return                      true is configuration must be updated
      //--------------------------------------------------------------
      bool needConfigurationUpdate(const IRfxcomConfiguration& configuration) const;

   private:
      //--------------------------------------------------------------
      /// \brief  The status type
      //--------------------------------------------------------------
      EStatusType m_statusType;

      //--------------------------------------------------------------
      /// \brief  The RFXCom type
      //--------------------------------------------------------------
      unsigned char m_rfxcomType;

      //--------------------------------------------------------------
      /// \brief  The RfxCom firmware version
      //--------------------------------------------------------------
      unsigned char m_firmwareVersion;

      //--------------------------------------------------------------
      /// \brief  The enable protocols
      //--------------------------------------------------------------
      bool m_AEenabled        ;
      bool m_RUBICSONenabled  ;
      bool m_FINEOFFSETenabled;
      bool m_LIGHTING4enabled ;
      bool m_RSLenabled       ;
      bool m_SXenabled        ;
      bool m_RFU6enabled      ;
      bool m_UNDECODEDenabled ;
      bool m_MERTIKenabled    ;
      bool m_LWRFenabled      ;
      bool m_HIDEKIenabled    ;
      bool m_LACROSSEenabled  ;
      bool m_FS20enabled      ;
      bool m_PROGUARDenabled  ;
      bool m_BLINDST0enabled  ;
      bool m_BLINDST1enabled  ;
      bool m_X10enabled       ;
      bool m_ARCenabled       ;
      bool m_ACenabled        ;
      bool m_HEEUenabled      ;
      bool m_MEIANTECHenabled ;
      bool m_OREGONenabled    ;
      bool m_ATIenabled       ;
      bool m_VISONICenabled   ;
   };
} // namespace rfxcomMessages
