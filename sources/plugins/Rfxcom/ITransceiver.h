#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "IRfxcomConfiguration.h"
#include <shared/communication/Buffer.hpp>
#include "rfxcomMessages/IRfxcomMessage.h"

namespace yApi = shared::plugin::yadomsApi;

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
   /// \return                      RFXCom message to send
   /// \throw shared::exception::CInvalidParameter if no corresponding RFXCom message was found (invalid command)
   //--------------------------------------------------------------
   virtual const shared::communication::CByteBuffer buildMessageToDevice(boost::shared_ptr<yApi::IYadomsApi> context, boost::shared_ptr<const yApi::IDeviceCommand> command) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Decode RFXCom message
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] data             Data received
   /// \return                      Decoded message, NULL if error when decoding
   //--------------------------------------------------------------
   virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(boost::shared_ptr<yApi::IYadomsApi> context, const shared::communication::CByteBuffer& data) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Process the manually device reaction request
   /// \param [in] context          Plugin execution context (Yadoms API)
   /// \param [in] data             Device manually creation data
   /// \return                      true if success
   //--------------------------------------------------------------
   virtual bool createDeviceManually(boost::shared_ptr<yApi::IYadomsApi> context, boost::shared_ptr<const yApi::IManuallyDeviceCreationData> data) const = 0;
};
