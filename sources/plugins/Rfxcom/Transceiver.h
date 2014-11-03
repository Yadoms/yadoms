#pragma once

#include "ITransceiver.h"
#include <shared/communication/IAsyncPort.h>
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
   virtual const shared::communication::CByteBuffer buildResetCmd() const;
   virtual const shared::communication::CByteBuffer buildGetStatusCmd() const;
   virtual const shared::communication::CByteBuffer buildSetModeCmd(unsigned char frequency, const IRfxcomConfiguration& configuration) const;
   virtual const shared::communication::CByteBuffer buildMessageToDevice(boost::shared_ptr<yApi::IYadomsApi> context, boost::shared_ptr<const yApi::IDeviceCommand> command) const;
   virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(boost::shared_ptr<yApi::IYadomsApi> context, const shared::communication::CByteBuffer& data) const;
   virtual bool createDeviceManually(boost::shared_ptr<yApi::IYadomsApi> context, boost::shared_ptr<const yApi::IManuallyDeviceCreationData> data) const;
   // [END] ITransceiver implementation
   
private:
   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;
};



