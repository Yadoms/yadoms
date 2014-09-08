#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "IRfxcomConfiguration.h"
#include "Buffer.hpp"
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
   virtual const CByteBuffer buildResetCmd() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom get status command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual const CByteBuffer buildGetStatusCmd() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom set mode command
   /// \param[in] frequency         RFXCom frequency
   /// \param[in] configuration     Protocols activations
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual const CByteBuffer buildSetModeCmd(unsigned char frequency, const IRfxcomConfiguration& configuration) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build a message to device
   /// \param [in] command          The received command (JSON string)
   /// \param [in] deviceParameters The device parameters (JSON string)
   /// \return                      RFXCom message to send
   /// \throw shared::exception::CInvalidParameter if no corresponding RFXCom message was found (invalid command)
   //--------------------------------------------------------------
   virtual const CByteBuffer buildMessageToDevice(const shared::CDataContainer& command, const shared::CDataContainer& deviceParametersTree) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Decode RFXCom message
   /// \param [in] data             Data received
   /// \return                      Decoded message, NULL if error when decoding
   //--------------------------------------------------------------
   virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(const CByteBuffer& data) const = 0;
};
