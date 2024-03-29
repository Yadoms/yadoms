﻿#pragma once
#include "shared/plugin/yPluginApi/IExtraQuery.h"
#include <shared/enumeration/EnumHelpers.hpp>

enum EIconSelectionMode
{
   kCustom = 0,
   kDefault
};

DECLARE_ENUM_HEADER(EDefaultIconType,
                           ((BrightnessDecrease))
                           ((BrightnessHigh))
                           ((BrightnessLow))
                           ((BrightnessMax))
                           ((BrightnessMin))
                           ((Duration))
                           ((Folder))
                           ((MediaMute))
                           ((MediaNextTrack))
                           ((MediaPlayPause))
                           ((MediaPrevTrack))
                           ((MediaStop))
                           ((MediaUnMute))
                           ((MediaVolumeDown))
                           ((MediaVolumeUp))
                           ((OpenWebsite))
                           ((SystemLaunch))
                           ((ToggleMicro))
                           ((ToggleMicroInactive))
                           ((MixerPostMessage))
                           ((MixerToggleSlowchat))
                           ((MixerToggleSlowchatInactive))
                           ((ToggleHotkey))
                           ((ToggleHotkeyOff))
                           ((On))
                           ((Off))
                           ((Plus))
                           ((Minus))
                           ((ColorBrightness))
                           ((ColorTemperature))
                           ((Contrast))
                           ((BroadcastOn))
                           ((BroadcastOff))
                           ((LightOff))
                           ((LightColor))
                           ((PartyPopper))
                           ((SystemBack))
);

class CDefaultIconSelector final
{
public:
   CDefaultIconSelector() = default;;
   virtual ~CDefaultIconSelector() = default;

   static std::vector<std::string> getAllDefaultIconNames();
   static std::string getIconPath(std::string& pluginPath, int& iconNameIndex);
 
private:

   static const std::string IconsImgPath;
   static const std::string BrightnessDecreasePath;
   static const std::string BrightnessHighPath;
   static const std::string BrightnessLowPath;
   static const std::string BrightnessMaxPath;
   static const std::string BrightnessMinPath;
   static const std::string DurationPath;
   static const std::string FolderPath;
   static const std::string MediaMutePath;
   static const std::string MediaNextTrackPath;
   static const std::string MediaPlayPausePath;
   static const std::string MediaPrevTrackPath;
   static const std::string MediaStopPath;
   static const std::string MediaUnMutePath;
   static const std::string MediaVolumeDownPath;
   static const std::string MediaVolumeUpPath;
   static const std::string OpenWebsitePath;
   static const std::string SystemLaunchPath;
   static const std::string ToggleMicroPath;
   static const std::string ToggleMicroInactivePath;
   static const std::string MixerPostMessagePath;
   static const std::string MixerToggleSlowchatPath;
   static const std::string MixerToggleSlowchatInactivePath;
   static const std::string ToggleHotkeyPath;
   static const std::string ToggleHotkeyOffPath;
   static const std::string OnPath;
   static const std::string OffPath;
   static const std::string PlusPath;
   static const std::string MinusPath;
   static const std::string ColorBrightnessPath;
   static const std::string ColorTemperaturePath;
   static const std::string ContrastPath;
   static const std::string BroadcastOnPath;
   static const std::string BroadcastOffPath;
   static const std::string LightOffPath;
   static const std::string LightColorPath;
   static const std::string PartyPopperPath;
   static const std::string SystemBackPath;
   
};
