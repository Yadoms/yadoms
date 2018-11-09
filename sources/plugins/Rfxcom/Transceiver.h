#pragma once

#include "ITransceiver.h"
#include "rfxcomMessages/IRfxcomMessage.h"
#include "ISequenceNumber.h"
#include "IUnsecuredProtocolFilter.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class implement the RFXCom protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{
public:
   explicit CTransceiver(boost::shared_ptr<IPairingHelper> pairingHelper);
   virtual ~CTransceiver();

   // ITransceiver implementation
   shared::communication::CByteBuffer buildResetCmd() const override;
   shared::communication::CByteBuffer buildGetStatusCmd() const override;
   shared::communication::CByteBuffer buildSetModeCmd(unsigned char frequency,
                                                      const IRfxcomConfiguration& configuration) const override;
   shared::communication::CByteBuffer buildStartReceiverCmd() const override;
   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                                          boost::shared_ptr<const yApi::IDeviceCommand> command) const
   override;
   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> buildRfyProgramMessage(
      const shared::communication::CByteBuffer& lastRequest) const override;
   boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                         const shared::communication::CByteBuffer& data) const override;
   std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const yApi::IManuallyDeviceCreationData& data) const override;
   void changeDeviceConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api,
                                  const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& deviceConfiguration) const override;
   // [END] ITransceiver implementation

private:

   static void logMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                          const boost::shared_ptr<rfxcomMessages::IRfxcomMessage>& message);

   static std::map<int, boost::shared_ptr<IUnsecuredProtocolFilter>> createUnsecuredProtocolFilters();

   boost::shared_ptr<IPairingHelper> m_pairingHelper;
   boost::shared_ptr<ISequenceNumber> m_seqNumberProvider;
   const std::map<int, boost::shared_ptr<IUnsecuredProtocolFilter>> m_unsecuredProtocolFilters;
};
