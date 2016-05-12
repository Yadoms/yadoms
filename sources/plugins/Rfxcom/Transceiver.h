#pragma once

#include "ITransceiver.h"
#include <shared/communication/IAsyncPort.h>
#include "rfxcomMessages/IRfxcomMessage.h"
#include "rfxcomMessages/RFXtrxHelpers.h"
#include "ISequenceNumberProvider.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

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
   virtual shared::communication::CByteBuffer buildResetCmd() const;
   virtual shared::communication::CByteBuffer buildGetStatusCmd() const;
   virtual shared::communication::CByteBuffer buildSetModeCmd(unsigned char frequency, const IRfxcomConfiguration& configuration) const;
   virtual shared::communication::CByteBuffer buildStartReceiverCmd() const;
   virtual boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command) const;
   virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(boost::shared_ptr<yApi::IYPluginApi> api, const shared::communication::CByteBuffer& data) const;
   virtual std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api, const yApi::IManuallyDeviceCreationData& data) const;
   // [END] ITransceiver implementation
   
private:
   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   boost::shared_ptr<ISequenceNumberProvider> m_seqNumberProvider;
};



