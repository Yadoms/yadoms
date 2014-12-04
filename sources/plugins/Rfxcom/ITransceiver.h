#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "IRfxcomConfiguration.h"
#include <shared/communication/Buffer.hpp>
#include "rfxcomMessages/IRfxcomMessage.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The RFXCom protocol interface
//--------------------------------------------------------------
class ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ITransceiver() {}

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom reset command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual const shared::communication::CByteBuffer buildResetCmd() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom get status command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual const shared::communication::CByteBuffer buildGetStatusCmd() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom set mode command
   /// \param[in] frequency         RFXCom frequency
   /// \param[in] configuration     Protocols activations
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual const shared::communication::CByteBuffer buildSetModeCmd(unsigned char frequency, const IRfxcomConfiguration& configuration) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build a message to device
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   /// \return                      RFXCom command to send (can be constitued of several messages)
   /// \throw shared::exception::CInvalidParameter if no corresponding RFXCom message was found (invalid command)
   //--------------------------------------------------------------
   virtual boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<const yApi::IDeviceCommand> command) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Decode RFXCom message
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] data             Data received
   /// \return                      Decoded message, NULL if error when decoding
   //--------------------------------------------------------------
   virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(boost::shared_ptr<yApi::IYPluginApi> context, const shared::communication::CByteBuffer& data) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Process the manually device reaction request
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] data             Device manually creation data
   /// \return                      Name of the created device
   /// \throw CManuallyDeviceCreationError   If error creating device
   //--------------------------------------------------------------
   virtual const std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> context, const yApi::IManuallyDeviceCreationData& data) const = 0;
};
