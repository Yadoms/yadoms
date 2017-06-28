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

   //--------------------------------------------------------------
   /// \brief	                           Get the frequency command
   /// \param [in] for433MHz              If true configure 433MHz, if false 868 MHz
   /// \param [in] enabled                If true enable the frequency, else disable
   /// \param [in] freq                   The chosen frequency (used only if enabled)
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildFrequencyCommand(bool for433MHz, bool enabled, const std::string & freq) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the selectivity command
   /// \param [in] for433MHz              If true configure 433MHz, if false 868 MHz
   /// \param [in] value                  The chosen selectivoty value (0 to 6)
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildSelectivityCommand(bool for433MHz, int value) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the sensivity command
   /// \param [in] for433MHz              If true configure 433MHz, if false 868 MHz
   /// \param [in] value                  The chosen sensivity value (0 to 4)
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildSensivityCommand(bool for433MHz, int value) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the DSP trigger command
   /// \param [in] for433MHz              If true configure 433MHz, if false 868 MHz
   /// \param [in] value                  The chosen trigger value (4 to 20 dBm)
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildDspTriggerCommand(bool for433MHz, int value) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the RFLink trigger command
   /// \param [in] for433MHz              If true configure 433MHz, if false 868 MHz
   /// \param [in] value                  The chosen trigger value (4 to 20 dBm)
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildRfLinkTriggerCommand(bool for433MHz, int value) const;

   //--------------------------------------------------------------
   /// \brief	                           Get the ListenBeforeTalk command
   /// \param [in] enable                 If true enable feature, else disable it
   /// \param [in] value                  The chosen lbt value (6 to 30 dBm)
   /// \return                            The command as string
   //--------------------------------------------------------------
   std::string buildLBTCommand(bool enable, int value) const;

};

