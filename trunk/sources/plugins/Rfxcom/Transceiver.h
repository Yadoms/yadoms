#pragma once

#include "ITransceiver.h"
#include "IAsyncPort.h"
#include "rfxcomMessages/IRfxcomMessage.h"
#include "rfxcomMessages/RFXtrxHelpers.h"
#include "ISequenceNumberProvider.h"

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	This class implement the RFXCom protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	                           Constructor
   //--------------------------------------------------------------
   CTransceiver();

   //--------------------------------------------------------------
   /// \brief	                           Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   virtual void reset();
   virtual const CByteBuffer buildResetCmd() const;
   virtual const CByteBuffer buildGetStatusCmd() const;
   virtual const CByteBuffer buildSetModeCmd(unsigned char frequency, const IRfxcomConfiguration& configuration) const;
   virtual const CByteBuffer buildMessageToDevice(const shared::CDataContainer& command, const shared::CDataContainer& deviceParametersTree) const;
   virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(const CByteBuffer& data) const;
   // [END] ITransceiver implementation
   
private:
   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;
};



