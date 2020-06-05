#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
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
   virtual ~ITransceiver()
   {
   }

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom reset command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual shared::communication::CByteBuffer buildResetCmd() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom get status command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual shared::communication::CByteBuffer buildGetStatusCmd() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom set mode command
   /// \param[in] frequency         RFXCom frequency
   /// \param[in] configuration     Protocols activations
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual shared::communication::CByteBuffer buildSetModeCmd(unsigned char frequency,
                                                              const IRfxcomConfiguration& configuration) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build the RFXCom start receiver command
   /// \return                      Buffer containing the command
   //--------------------------------------------------------------
   virtual shared::communication::CByteBuffer buildStartReceiverCmd() const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build a message to device
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] command          The received command
   /// \return                      RFXCom command to send (can be constituted of several messages)
   /// \throw shared::exception::CInvalidParameter if no corresponding RFXCom message was found (invalid command)
   //--------------------------------------------------------------
   virtual boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                                                  boost::shared_ptr<const yApi::IDeviceCommand>
                                                                                                  command) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Build a RFY-specific message to device
   /// \param [in] lastRequest      Last RFY request who failed because of unknown remote
   /// \return                      RFXCom command to send (can be constituted of several messages)
   //--------------------------------------------------------------
   virtual boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> buildRfyProgramMessage(
      const shared::communication::CByteBuffer& lastRequest) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Decode RFXCom message
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] data             Data received
   /// \return                      Decoded message, NULL if error when decoding
   //--------------------------------------------------------------
   virtual boost::shared_ptr<rfxcomMessages::IRfxcomMessage> decodeRfxcomMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                                 const shared::communication::CByteBuffer& data) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Process the manually device reaction request
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] data             Device manually creation data
   /// \return                      Name of the created device
   /// \throw CManuallyDeviceCreationError   If error creating device
   //--------------------------------------------------------------
   virtual std::string createDeviceManually(boost::shared_ptr<yApi::IYPluginApi> api,
                                            const yApi::IManuallyDeviceCreationData& data) const = 0;

   //--------------------------------------------------------------
   /// \brief	                     Process the change device configuration request
   /// \param [in] api              Plugin execution context (Yadoms API)
   /// \param [in] deviceConfiguration The new device configuration
   //--------------------------------------------------------------
   virtual void changeDeviceConfiguration(const boost::shared_ptr<yApi::IYPluginApi>& api,
                                          const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& deviceConfiguration) const = 0;
};
