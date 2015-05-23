#pragma once
#include "TeleInfotrxHelpers.h"
#include "ITeleInfoMessage.h"
#include "ITeleInfoConfiguration.h"

//TODO : A nettoyer !!

namespace TeleInfoMessages
{
   //--------------------------------------------------------------
   /// \brief	This class manage the TeleInfo status
   //--------------------------------------------------------------
   class CTransceiverStatus : public ITeleInfoMessage
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
      /// \param[in] rbuf                    Status received from TeleInfo Receiver
      //--------------------------------------------------------------
      CTransceiverStatus( 
		                  const RBUF& rbuf, 
		                  boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider 
						);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CTransceiverStatus();

      // ITeleInfoMessage implementation
      virtual boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
      virtual void reset();
      virtual const std::string& getDeviceName() const;
      // [END] ITeleInfoMessage implementation

      //--------------------------------------------------------------
      /// \brief	                     Trace the TeleInfo Receiver configured protocols
      //--------------------------------------------------------------
      void traceEnabledProtocols() const;

      //--------------------------------------------------------------
      /// \brief	                     Get the status type (= message subType)
      // \return                       The status type
      //--------------------------------------------------------------
      EStatusType getStatusType() const;

      //--------------------------------------------------------------
      /// \brief	                     Get the TeleInfo Receiver type
      // \return                       The TeleInfo Receiver type
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
      //bool needConfigurationUpdate(const IRfxcomConfiguration& configuration) const;

   private:
      //--------------------------------------------------------------
      /// \brief  The status type
      //--------------------------------------------------------------
      EStatusType m_statusType;

      //--------------------------------------------------------------
      /// \brief  The RFXCom type
      //--------------------------------------------------------------
      //unsigned char m_rfxcomType;

      //--------------------------------------------------------------
      /// \brief  The RfxCom firmware version
      //--------------------------------------------------------------
      unsigned char m_firmwareVersion;

      //--------------------------------------------------------------
      /// \brief  The enable protocols
      //--------------------------------------------------------------
      bool m_PIREnabled;
      bool m_MainUnitEnabled;
   };
} // namespace TeleInfoMessages
