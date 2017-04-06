#pragma once
#include "RFXtrxHelpers.h"
#include "IRfxcomMessage.h"

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	This class manage an acknoledge from RFXCom
   //--------------------------------------------------------------
   class CAck : public IRfxcomMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] rbuf                    Message received from Rfxcom
      /// \param[in] rbufSize                Message size, received from Rfxcom
      /// \param[in] seqNumberProvider       The sequence number provider
      //--------------------------------------------------------------
      CAck(const RBUF& rbuf, size_t rbufSize,
           boost::shared_ptr<ISequenceNumber> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CAck();

      // IRfxcomMessage implementation
      boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const override;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const override;
      const std::string& getDeviceName() const override;
      // [END] IRfxcomMessage implementation

      //--------------------------------------------------------------
      /// \brief	                           Get the ack state
      /// \return                            The ack state
      //--------------------------------------------------------------
      bool isOk() const
      {
         return m_ack;
      }

   private:
      //--------------------------------------------------------------
      /// \brief  The ack state
      //--------------------------------------------------------------
      bool m_ack;
   };
} // namespace rfxcomMessages


