#pragma once

#include <shared/communication/StringBuffer.h>
#include "Configuration.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationData.h>

class CTransceiver 
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

   //--------------------------------------------------------------
   /// \brief	                           Get the hello command
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildHelloCmd() const;

   //--------------------------------------------------------------
   /// \brief	                           Get the start listening command
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildStartListeningData() const;

   //--------------------------------------------------------------
   /// \brief	                           Get the receiver configiuration command
   /// \param [in] protocols              The activated protocols container
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildReceiverConfigurationCommand(const CConfiguration::CProtocols & protocols) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the repeater on/off command
   /// \param [in] repeaterActive         The repeater active status
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildRepeaterActivationCommand(bool repeaterActive) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the repeater configiuration command
   /// \param [in] protocols              The activated protocols container
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildRepeaterConfigurationCommand(const CConfiguration::CProtocols & protocols) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the led activity command
   /// \param [in] ledActivity            The led activity status
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildLedActivityCommand(bool ledActivity) const;
};

