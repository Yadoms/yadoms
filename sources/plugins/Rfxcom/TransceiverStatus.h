#pragma once
#include "rfxcomMessages/RFXtrxHelpers.h"
#include "RfxComConfiguration.h"
//#include "IRfxcomConfiguration.h"


//--------------------------------------------------------------
/// \brief	This class manage the RFXCom status
//--------------------------------------------------------------
class CTransceiverStatus
{  
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   /// \param[in] rbuf                    Status received from rfxcom
   //--------------------------------------------------------------
   CTransceiverStatus(const RBUF& rbuf);

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiverStatus();

   //--------------------------------------------------------------
   /// \brief	                     Trace the RFXCom configured protocols
   //--------------------------------------------------------------
   void traceEnabledProtocols() const;

   //--------------------------------------------------------------
   /// \brief	                     Get the RfxCom type
   // \return                       The RfxCom type
   //--------------------------------------------------------------
   unsigned char getRfxcomType() const;

   //--------------------------------------------------------------
   /// \brief	                     Get the RfxCom type as string
   // \return                       A string reprenting the RfxCom type
   //--------------------------------------------------------------
   const std::string rfxcomTypeToString() const;

   //--------------------------------------------------------------
   /// \brief	                     Get the RfxCom type as string
   // \return                       A string reprenting the RfxCom type
   //--------------------------------------------------------------
   unsigned char getFirmwareVersion() const;

   //--------------------------------------------------------------
   /// \brief	                     Check if RfxCom configuration must be updated (need protocols enabled for example)
   /// \parame[in] configuration    The configured RfxCom status
   /// \return                      true is configuration must be updated
   //--------------------------------------------------------------
   bool needConfigurationUpdate(const IRfxcomConfiguration& configuration) const;

private:
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
