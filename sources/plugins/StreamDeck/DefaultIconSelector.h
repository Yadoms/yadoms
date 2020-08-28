#pragma once
#include "shared/plugin/yPluginApi/IExtraQuery.h"

enum EIconSelectionMode
{
   kCustom = 0,
   kDefault
};

class CDefaultIconSelector
{
public:
   CDefaultIconSelector() = default;;
   virtual ~CDefaultIconSelector() = default;

   static EIconSelectionMode getIconSelectionMode(
      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery>& extraQuery);
private:
   static const std::string BrightnessDecrease;
   static const std::string BrightnessHigh;
   static const std::string BrightnessLow;
   static const std::string BrightnessMax;
   static const std::string BrightnessMin;
   static const std::string Duration;
   static const std::string Folder;
   static const std::string MediaMute;
   static const std::string MediaNextTrack;
   static const std::string MediaPlayPause;
   static const std::string MediaPrevTrack;
   static const std::string MediaStop;
   static const std::string MediaUnMute;
   static const std::string MediaVolumeDown;
   static const std::string MediaVolumeUp;
   static const std::string OpenWebsite;
   static const std::string SystemLaunch;
   static const std::string ToggleMicro;
   static const std::string ToggleMicroInactive;
   static const std::string MixerPostMessage;
   static const std::string MixerToggleSlowchat;
   static const std::string MixerToggleSlowchatInactive;
   static const std::string ToggleHotkey;
   static const std::string ToggleHotkeyOff;
   static const std::string On;
   static const std::string Off;
   static const std::string Plus;
   static const std::string Minus;
   static const std::string ColorBrightness;
   static const std::string ColorTemperature;
   static const std::string Contrast;
   static const std::string BroadcastOn;
   static const std::string BroadcastOff;
   static const std::string LightOff;
   static const std::string LightColor;
   static const std::string PartyPopper;
   static const std::string SystemBack;
};
