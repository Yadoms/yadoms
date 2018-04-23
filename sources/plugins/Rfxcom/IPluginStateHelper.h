#pragma once

//--------------------------------------------------------------
/// \brief	Helper managing the plugin state
//--------------------------------------------------------------
class IPluginStateHelper
{
public:
   enum EState
   {
      kStopped,
      kRunning,
      kError,
      kConnecting,
      kInitializing,
      kConnectionLost,
      kUpdateConfiguration,
      kFailToConfigure,
      kUpdateFirmware,
      kStartPairing,
      kStopPairing
   };

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IPluginStateHelper()
   {
   }

   //--------------------------------------------------------------
   /// \brief     Set the internal plugin state
   /// \param[in] internalState  Plugin state
   /// \param[in] customMessageId (optional) Message for certain states (for GUI)
   /// \param[in] customMessageId (optional) Message data for certain states (for GUI)
   //--------------------------------------------------------------
   virtual void set(EState internalState,
                    const std::string& customMessageId = std::string(),
                    const std::map<std::string, std::string>& customMessageDataParams = std::map<std::string, std::string>()) = 0;
};
