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
      /// \param[in] seqNumberProvider       The sequence number provider
      /// \param[in] rbuf                    Message received from Rfxcom
      //--------------------------------------------------------------
      CAck(const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CAck();

      // IRfxcomMessage implementation
      virtual boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const;
      virtual void historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const;
      virtual const std::string& getDeviceName() const;
      // [END] IRfxcomMessage implementation

      //--------------------------------------------------------------
      /// \brief	                           Get the ack state
      /// \return                            The ack state
      //--------------------------------------------------------------
      bool isOk() const { return m_ack; }

   private:
      //--------------------------------------------------------------
      /// \brief  The ack state
      //--------------------------------------------------------------
      bool m_ack;
   };

} // namespace rfxcomMessages
